// gcc unlink.c -o unlink -ggdb -no-pie

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *chunk1, *chunk2;

int main()
{
	chunk1 = (char *)malloc(0x20);
	chunk2 = (char *)malloc(0x410);

	printf("chunk1 -> %lx\n", (int64_t)chunk1);
	*(int64_t *)(chunk1+0x8 ) = 0x20;
	*(int64_t *)(chunk1+0x10) = (int64_t)&chunk1-0x18;
	*(int64_t *)(chunk1+0x18) = (int64_t)&chunk1-0x10;
	*(int64_t *)(chunk2-0x10) = 0x20;
	*(int64_t *)(chunk2-0x8 ) = 0x420;

	free(chunk2);
	printf("chunk1 -> %lx\n", (int64_t)chunk1);
}