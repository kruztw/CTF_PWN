// gcc simple.c -o simple -lpthread -no-pie -ggdb -z lazy
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

char *ptr;
char note[0x100];
int count;

void *write_note(void *buf)
{
    if(count < 0x100){
        for(int i = 0; i<8; ++i){
            *ptr = ((char *)buf)[i];
            ++ptr;
        }
        count += 8;
    }
    pthread_exit(NULL);
}

void *delete_note()
{
    count -= 8;
    ptr -= 8;              // 元兇
    sleep(1);              // race condition
    if(count < 0){
      count = 0;
      ptr += 8;
    }

    pthread_exit(NULL);
}

void win()
{
  system("/bin/sh");
}

int main()
{
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    char buf[0x8];
    ptr = note;
    while(1){
        int op;
        printf("1.new\n2.del\n3.exit\n>"); 
        scanf("%d", &op);
        switch(op){
            case 1:{
                printf("data:");         // 如果不在 main 讀取, 會演變成 main 和 write_note 搶輸入的 race condition
                read(0, buf, 0x8);
                pthread_t t;
                pthread_create(&t, NULL, write_note, buf);
                break;
            }
            case 2:{
                pthread_t t;
                pthread_create(&t, NULL, delete_note, NULL);
                break;
            }
            case 3:
                return 0;
        }
    }
}