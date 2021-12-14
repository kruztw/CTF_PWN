// gcc guess.c -o guess -ggdb -no-pie -z execstack

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

char flag[0x100];

void read_flag()
{
	int fd = open("./flag.txt", O_RDONLY);
	read(fd, flag, 0x100);
	close(fd);
}

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);

	read_flag();
	
	void (*fptr)();
	char code[0x100];
	printf("give me your shell code:");
	read(0, code, sizeof(code));

	fptr = (void (*)())&code;
	fptr();
}