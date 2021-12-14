#include <stdint.h>
#include <x86intrin.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0
#define PAGE_SIZE 4096

char array[256*PAGE_SIZE];
char flag[] = "flag{secret}";

int measure(int idx)
{
    uint64_t start, end, collapse;
    char tmp = array[flag[idx]*PAGE_SIZE];
    int min_time = 10000, min_idx = -1;

    for (int i = 0; i < 256; i++) {
        int idx = ((i * 167) + 13) & 0xff;
        if (idx == 0)                      // 可能是因為 base + offset 導致 &array[0] 總是會被 load 進來
            continue;

        start = __rdtsc();
        tmp = array[idx*PAGE_SIZE];      
        _mm_lfence();
        end = __rdtsc();
        _mm_lfence();
        collapse = end - start;
        
        if (DEBUG)
            printf("%d page: latency = %lu \n", idx, collapse);
    
        if (collapse < min_time) {
            min_time = collapse;
            min_idx = idx;
        }  
    }

    if (DEBUG)
        printf("min_idx = %d\n", min_idx);
    return min_idx;
}

int main()
{
    char ans[0x100] = {0};
    int score[256];
    int max_score, max_idx;

    /* will bring array in the cache */
    for (int i = 0; i < sizeof(array); i++)
        array[i] = i;

    for (int i = 0; i < sizeof(flag); i++) {
        memset(score, 0, sizeof(score));
        for (int t = 0; t < 100; t++) {
            for (int j = 0; j < 256; j++) {
                  _mm_mfence();                      /* prevent clflush from being reordered by the CPU or the compiler in this direction */
                  _mm_clflush( &array[j*PAGE_SIZE] );
            }
            score[measure(i)]++;
        }

        max_score = 0, max_idx = 0;
        for (int k = 0; k < 256; k++)
            if (score[k] > max_score) {
                max_score = score[k];
                max_idx = k;
            }

        ans[i] = max_idx;
    }

    printf("flag = %s\n", ans);
    return 0;
}
 
