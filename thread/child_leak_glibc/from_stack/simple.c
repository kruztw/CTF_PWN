// gcc simple.c -lpthread -ggdb -o simple
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* child(void* arg)
{
    int a;
    printf("%llx\n", &a);
}


int main() {
    pthread_t t; 
    pthread_create(&t, NULL, child, NULL);
    pthread_join(t, NULL);
}
