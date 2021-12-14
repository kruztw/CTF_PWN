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
	char fmt[13];
	for(int i = 0; i<3; i++){
		printf("give me fmt:");
		read(0, fmt, 13);
		printf(fmt);
	}
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