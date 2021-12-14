#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
    char *addr;
    printf("exit @ %p\n", exit);
    printf("addr:\n");
    scanf("%ld", &addr);
    printf("content:\n");
    read(0, addr, 0x10000);
    exit(0);
}
