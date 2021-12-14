#include <stdio.h>
#include <stdint.h>

char msg[100];
int main()
{
    setvbuf(stdout, 0, 2, 0);
    puts("hello"); // or stdout->_flag |= 0x800;
    stdout->_IO_read_end = &msg;
    stdout->_IO_write_base = &msg;
    stdout->_IO_write_ptr = &msg;
    stdout->_IO_write_end  = &msg+100;
    puts("hello");
    puts(msg);
}
