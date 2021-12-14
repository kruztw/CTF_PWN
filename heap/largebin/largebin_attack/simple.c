#include <stdio.h>
#include <stdlib.h>

size_t buf[0x10];

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);  
    size_t *ptr, *ptr2, *ptr3;

    ptr = malloc(0x438);
    malloc(0x18);
    ptr2 = malloc(0x448);
    malloc(0x18);
    free(ptr);
    // put ptr into large bin
    malloc(0x600);
    free(ptr2);
    ptr[2] = 0;                          // fd
    ptr[3] = (size_t)&buf[0];            // bk

    printf("buf[4]: 0x%lx\n", buf[4]);

    /*
     * unsorted bin -> ptr2
     * largebin     -> ptr
     * 先將 ptr2 放到 largebin => 
     * ptr2->fd_nextsize = ptr , ptr2->bk_nextsize = ptr->bk_nextsize
     * ptr->bk_nextsize = ptr2 , ptr2->bk_nextsize->fd_nextsize = ptr2
     */

    ptr3 = malloc(0x68);
    printf("buf[4]: 0x%lx\n", buf[4]);

    return 0;
}
