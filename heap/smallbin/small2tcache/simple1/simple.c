// gcc small2tcache.c -o small2tcache -ggdb
// https://elixir.bootlin.com/glibc/glibc-2.29/source/malloc/malloc.c#L3673

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

	clear_tcache(0x300, 7);
	// move chunkAB to small bin
	malloc(0x500);
	// move chunkA to tcache
	malloc(0x300);
}
