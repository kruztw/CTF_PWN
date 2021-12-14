/* libc: 2.31 */
#include <stdio.h>
#include <unistd.h>

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    char buff[0x10];
    printf(":");
    read(0, buff, 0x8*6);
    puts(buff);
    printf(":");
    read(0, buff, 0x8*6);
}
