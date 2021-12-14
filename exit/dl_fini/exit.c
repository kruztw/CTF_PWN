#include <stdio.h>
#include <stdlib.h>

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	int64_t *addr;
	printf("exit @ %p\n", (void *)&exit);
	printf("give me an address: ");
	scanf("%ld", (int64_t *)&addr);
	printf("value: ");
	scanf("%ld", addr);
	exit(0);
}
