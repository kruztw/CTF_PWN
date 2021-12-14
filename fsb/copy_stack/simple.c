#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main()
{
	srand(time(NULL));
	int64_t from = rand()&0xffff;
	int64_t to = 0x6666;
	int64_t *ptr = &to;

	char buf[10];
	scanf("%10s", buf);
	printf(buf);

	if(from == *ptr)
		system("/bin/sh");
}