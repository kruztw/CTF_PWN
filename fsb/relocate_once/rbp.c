// gcc ./rbp.c -o rbp
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void win()
{
	system("/bin/sh");
}

void foo2()
{
	char fmt[0x100];
	printf("give me fmt:");
	read(0, fmt, 0x100);
	printf(fmt);
}

void foo1()
{
	foo2();
}

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	foo1();
}