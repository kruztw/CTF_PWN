#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

int main()
{
	int fd = open("./tmp_file", O_RDWR);
	char op;

	char *map = mmap(0, 0x1000, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	read(0, map, 0x100);

	if(!strncmp(map, "sh", 2))
		goto exit;

	printf("Sure?(y/n)");
	scanf("%c", &op);

	if(op == 'y' && !strncmp(map, "sh", 2))
		printf("get shell\n");

	exit:
		munmap(map, 0x1000);
		close(fd);
}