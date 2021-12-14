#include <stdio.h>
#include <unistd.h>

char code[0x1000];

int main()
{
	read(0, code, 0x1000);
    (*(void(*)()) code)();
}