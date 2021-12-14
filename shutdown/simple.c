// gcc simple.c -o simple -no-pie -masm=intel -fno-stack-protector
#include <unistd.h>

void gift(){
	__asm__(
		"pop rax;"
		"ret;"
		"add qword ptr [rdi], rax;"
		"ret;"
		:::"rax"
	);
}

char file[]="flag.txt";

int main()
{
	char buf[0x10];
	while(read(0, buf, 0x100));
}