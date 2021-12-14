#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>


char buf[0x100];

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	
	char *ptr = dlsym(RTLD_NEXT, "_IO_list_all");
	printf("_IO_list_all @ %p\n", ptr);
	printf("buf:");
	gets(buf);
	printf("addr:");
	scanf("%s", ptr);
}
