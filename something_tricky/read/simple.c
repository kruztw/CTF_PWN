'''
Since read is unbuffered, we don't care about spoiling stdin, which means we can overflow it and control malloc_hook and main_arena. 
'''

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void target()
{
    puts("You win");
}

int main()
{
    printf("target @ %lp\n", target);
    read(0, stdin, 0x1000);
    malloc(0x10);
}
