#define _GNU_SOURCE
#include <stdio.h>
#include <dlfcn.h>


char buf[0x100];
char buf2[0x100];
char table[0x100];
char table2[0x100];

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);
	
	char *ptr = dlsym(RTLD_NEXT, "_IO_list_all");
	printf("_IO_list_all @ %p\n", ptr);
	printf("buf:");
	gets(buf);
	printf("buf2:");
	gets(buf2);
	printf("table:");
	gets(table);
	printf("table2:");
	gets(table2);
	printf("addr:");
	scanf("%s", ptr);
}
