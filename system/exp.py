"""
第一個 gdb 用來證實暫存器不影響 (寫在 gdb.attach 沒反應, 請自行 copy 到 gdb 執行)
set $rax=$rbx=$rcx=$rdx=$rsi=$rbp=$r8=$r9=$r10=$r11=$r12=$r13=$r14=$r15=0xfaceb00c
set $rsp=$_heap()+0x808
set $rsp=$_bss()+0x808

# 因為 movaps 所以 $rsp 一定要以 8 結尾, 而且不能離邊界太近, 否則會 segfault

第二個 gdb 用來觀察
最後兩個參數最重要,分別代表 argv 和 envp, 只要這兩個值對了, 不管暫存器的值多麼可笑, 都會成功
*envp 指向錯誤位址 => clone 後 ec=0xe (繼續 si 下去就會看到了, 在 CLONE 的最後一個參數 args 包含 argv 和 envp, 0xe => bad address)
"""

from pwn import *

r = process('./simple')

gdb.attach(r, 'b system\nc')
#gdb.attach(r, 'b system\nc\nn 24\nsi 15')

r.interactive()
