// gcc realloc_hook.c -o realloc_hook
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

char *sh = "/bin/sh";

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	void *chunk = malloc(0x30);
	printf("printf @ %p\n", &printf);
	printf("Give me an address:");

	void *ptr = NULL;
	scanf("%ld", (int64_t *)&ptr);
	printf("value:");
	read(0, ptr, sizeof(ptr));

	chunk = realloc(chunk, 0x40);
}
