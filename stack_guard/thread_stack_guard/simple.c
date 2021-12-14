#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void shell()
{
    system("/bin/sh");
}

void *win(void *arg)
{
    char buf[0x10];
    read(0, buf, 0x1000);
}

int main()
{
    pthread_t thread;
    pthread_create(&thread, NULL, win, NULL);
    pthread_join(thread, NULL);
}
