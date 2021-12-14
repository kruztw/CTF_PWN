#include <stdio.h>
#include <stdint.h>

char msg[10];
int main()
{
    setvbuf(stdin, 0, 2, 0);
    stdin->_IO_buf_base = msg;
    stdin->_IO_buf_end = msg+10;
    scanf("%s");
    puts(msg);
}
