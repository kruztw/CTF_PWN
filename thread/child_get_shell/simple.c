// gcc simple.c -lpthread -ggdb -o simple
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void win()
{
    system("/bin/sh");
}

void* child(void* arg)
{
    int64_t vuln[1];
    vuln[3] = &win+22; // ret
    vuln[4] = &win;
}


int main() {
    pthread_t t; 
    pthread_create(&t, NULL, child, NULL);
    pthread_join(t, NULL);
}
