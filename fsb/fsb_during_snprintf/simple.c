#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char *cmd = "echo bye";
char *sh = "sh";

int main()
{
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);

	// compiler 會重排位置, 所以要先宣 b 再宣告 fmt
	// 利用 b bof 到 fmt
	// 再用 fsb 讓 cmd 指到 sh

	char b[0x10] = {};		  		  // rbp-0x130
	char fmt[0x10] = "%s";            // rbp-0x120
	char a[0x100] = {};               // rbp-0x110

	printf("buf:");
	read(0, a, 0x100);
	snprintf(b, 0x100, fmt, a);

	system(cmd);
}