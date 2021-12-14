#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	FILE *fd = fopen("/proc/self/mem", "rw+");
	if(!fd)
		return -1;

	int64_t addr;
	char buf[0x100];
	printf("addr:");
	scanf("%ld", &addr);
	printf("buf:");
	read(0, buf, sizeof(buf));

	fseek(fd, addr, SEEK_SET);
	fprintf(fd, "%s", buf);
	fclose(fd);
	printf("done");
}