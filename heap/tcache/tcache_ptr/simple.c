/*
 *  overwrite tcache_ptr in libc to fabricate our tcache
 *  libc: 2.27
 */


#include <stdio.h>
#include <stdlib.h>

char my_tcache[0x800];

int main() {

    setvbuf(stdin, 0, 2,0);
    setvbuf(stdout, 0, 2, 0);
    free(malloc(0x10));                  // let me know how to fill my_tcache

    printf("printf @ 0x%lx\n", &printf);
    printf("my_tcache @ 0x%lx\n", my_tcache);
    char *tcache_ptr;

    printf("tcache_ptr addr:");
    scanf("%ld", &tcache_ptr);
    printf("point to:");
    scanf("%ld", tcache_ptr);

    printf("make up my_tcache:");
    read(0, my_tcache, 0x800);

    if (malloc(0x10) == 0x601800)  // 0x601800 means nothing but we can allocate any chunk address.
        printf("You win !\n");
    else
        printf("Nope\n");
}
