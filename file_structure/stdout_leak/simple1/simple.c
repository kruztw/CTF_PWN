// libc: 2.27
#include <stdio.h>
#include <stdint.h>

int main()
{
	setvbuf(stdout, 0, 2, 0);
	//https://elixir.bootlin.com/glibc/glibc-2.27/source/libio/fileops.c#L439
	stdout->_flags = 0x1800;
	stdout->_IO_write_base = (char *)(((int64_t)stdout->_IO_write_base>>8)<<8);
	printf("hello");
}
