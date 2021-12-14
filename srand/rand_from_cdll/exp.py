#!/usr/bin/python
# encoding: utf-8
from pwn import *
from ctypes import *
from time import time

cdll.LoadLibrary("libc.so.6")
libc = CDLL("libc.so.6")
seed = int(time())
libc.srand(seed)
libc.rand()