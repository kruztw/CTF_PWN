// libc: 2.31
#include <stdio.h>
#include <stdint.h>

/* if target size too small, it may overwrite stdout_ptr and trigger exception (e.g. target[] ) */
char target[0x100] = "Wow You leak me\n";
int main() 
{
    //setvbuf(stdout, 0, 2, 0);
    puts("init stdout");
    stdout->_flags |= 0x1800;
    stdout->_IO_buf_base = target+0x10;
    /* https://elixir.bootlin.com/glibc/glibc-2.31/source/libio/fileops.c#L453 */
    puts("now _IO_write_ptr = target + 0x10");
    stdout->_IO_write_base = target;
    /* _IO_flush_all will flush the data, but when exploit this puts is not the last command, so keep it !*/
    puts("finished!\n");
}

