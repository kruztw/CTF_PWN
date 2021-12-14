// libc: 2.29
#include <stdio.h>
#include <stdint.h>

int main() 
{
    setvbuf(stdout, 0, 2, 0);
    //https://elixir.bootlin.com/glibc/glibc-2.29/source/libio/fileops.c#L448
    stdout->_flags |= 0x800;
    stdout->_IO_write_ptr = (int64_t)stdout->_IO_write_ptr | 0xff;
    puts("hello");
}

