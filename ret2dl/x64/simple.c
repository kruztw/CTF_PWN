// gcc simple.c -o simple -fno-stack-protector -no-pie -ggdb
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

char buf[0x200];
int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	printf("setvbuf @ %lx\n", (int64_t)setvbuf);

	int64_t *addr;
	printf("give me one address, let's clear it!!\n");
	printf("addr:");
	scanf("%lx", (int64_t *)&addr);
	*addr = 0;

	write(1, "buf:", 4);
	read(0, buf, 0x200);

	char a;
	write(1, "a:", 2);
	read(0, &a, 0x100);
}