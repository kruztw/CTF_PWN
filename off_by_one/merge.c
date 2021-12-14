#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct greet
{
	char hello[0x18];
	char name[0x10];
}greet;

void fillin_tache(const int size)
{
	char *ptr[7];
	for(int i = 0; i<7; i++)
		ptr[i] = malloc(size);
	for(int i = 0; i<7; i++)
		free(ptr[i]);
}

int main()
{
	setvbuf(stdin, 0, 2, 0);
	setvbuf(stdout, 0, 2, 0);


	char  *chunk1 = (char *)malloc(0x418);
	greet *go     = (greet*)malloc(sizeof(greet));
	char  *chunk3 = (char *)malloc(0x4f0);
	
	// 先 free , 免得等會 off-by-one 觸發 unlink 造成 corrupted size vs. prev_size
	// 因為 chunk1 free 後會問下下一塊 "上一塊是否 merge ?"
	// 倘若先 off-by-one, 就會 merge, 並檢查 chunksize (p) != prev_size (next_chunk (p)) ?
	// 當然可以繞過 (將 go 的 prev_size 寫入 chunk1 size), 但為了簡潔所以省略
	free(chunk1);

	snprintf(go->hello, sizeof(go->hello), "%s", "echo hello guys");
	printf("name:");
	go->name[read(0, go->name, sizeof(go->name))] = 0;

	// 順利的話這時候要 merge 
	free(chunk3);

	// 連同 go 一起 malloc 出來
	char *message = (char *)malloc(0x450);
	printf("message:");
	read(0, message, 0x440);

	system(go->hello);
}