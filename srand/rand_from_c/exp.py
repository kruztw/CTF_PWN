from pwn import *
from time import *
import os

t = int(time())
captcha = "123"
rand = os.popen("./myhash " + str(t) + " " + captcha).read().strip()
print "rand = ", rand