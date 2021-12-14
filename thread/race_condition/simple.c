// gcc simple.c -lpthread -ggdb -o simple
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

char target[0x10];

void* child1(void* arg)
{
    strcpy(target, "from child1");
    sleep(1);
    printf("target = %s\n", target);
}

void *child2(void *arg)
{
    strcpy(target, "child2 hacked");
}


int main() {
    pthread_t t1, t2; 
    pthread_create(&t1, NULL, child1, NULL);
    pthread_create(&t2, NULL, child2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}
