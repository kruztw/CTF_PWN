// gcc simple.c -lpthread -ggdb -o simple
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* child(void* arg)
{
    int64_t *p = malloc(0x10);
    printf("main_arena @ %llx\n", *(p-0x2d0/8));
}


int main() {
    pthread_t t; 
    pthread_create(&t, NULL, child, NULL);
    pthread_join(t, NULL);
}
