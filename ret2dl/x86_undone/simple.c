// gcc simple.c -o simple -fno-stack-protector -no-pie -ggdb -m32
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

char buf[0x200];
int main()
{
	write(1, "buf:", 4);
	read(0, buf, 0x200);

	char a;
	write(1, "a:", 2);
	read(0, &a, 0x100);
}