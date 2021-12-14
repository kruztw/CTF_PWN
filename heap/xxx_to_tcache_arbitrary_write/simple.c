#include <stdio.h>
#include <stdlib.h>

int main()
{
    int64_t *ptr, *ptr1;
    ptr  = malloc(0x10);
    ptr1 = malloc(0x10);

    free(ptr);
    ptr[0] = 0xfaceb00c;
    ptr = malloc(0x10);
    free(ptr1);
    printf("*ptr1 = 0x%lx\n", *ptr1);
}
