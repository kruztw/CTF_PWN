#include <stdio.h>
#include <stdlib.h>

int main()
{
    *(int64_t *)(&stdout) = stderr;
    close(1);
    printf("hello\n");
}
