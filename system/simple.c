#include <stdlib.h>
#include <unistd.h>

int main()
{
    sleep(1);
    malloc(0x10);
    system("/bin/sh");
}
