#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int64_t target;
int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	int64_t *chunk = malloc(0x10);
	*(chunk+3) = -1;

	malloc( -((int64_t)chunk+0x10) + (int64_t)&target - 0x20);

	// top chunk is at target right now.
	chunk = malloc(0x8);
	*chunk = 0xfaceb00c;

	printf("target = 0x%lx\n", target);
}