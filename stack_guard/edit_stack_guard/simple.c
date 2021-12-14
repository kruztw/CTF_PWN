#include <stdio.h>
#include <stdlib.h>

int main()
{
	int64_t address;
	char buf[0x10];
	char cmd[0x10] = "echo hi";

	setvbuf(stdin, 0, _IONBF, 0);
	setvbuf(stdout, 0, _IONBF, 0);

	printf("setvbuf @ %p\n", setvbuf);
	printf("address:");
	scanf("%lx", &address);
	getchar();
	*(int64_t *)address = 0;


	printf("buf:");
	gets(buf);
	system(cmd);
}