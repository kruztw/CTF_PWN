#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>

char rop[0x1000];
int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	printf("setvbuf @ 0x%lx\n", (int64_t)setvbuf);
	printf("rop:");
	read(0, rop, 0x1000);

	char ch[0x10];
	printf("ch:");
	read(0, ch, 0x30);
}