// gcc fast2tcache.c -o fast2tcache -ggdb

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
	char *chunkA = (char *)malloc(0x30);
	char *chunkB = (char *)malloc(0x30);
	fillin_tcache(0x30);

	free(chunkB);
	free(chunkA);

	clear_tcache(0x30, 7);
	// move chunkB to tcache
	malloc(0x30);
}
