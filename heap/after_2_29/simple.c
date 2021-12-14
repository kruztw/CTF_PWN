#include <stdlib.h>

const int size = 0x90;

/*
tcache: 1->2->3->4->5->6->7
unsorted_chunk: 0

malloc
tcache: 2->3->4->5->6->7
unsorted_chunk: 0

free
tcache: 0->2->3->4->5->6->7
unsorted_chunk: 0


tcache: 0->1->2->3->4->5->6
unsorted_chunk:

free(0) : double free detected !

*/

int main()
{
    char *ptr[0x10];
    for (int i = 0; i < 8; i++)
        ptr[i] = malloc(size);

   for (int i = 7; i >= 0; i--)
       free(ptr[i]);

   ptr[15] = malloc(size);
   free(ptr[0]);
}
