/* libc: 2.31 */
#include <stdio.h>
#include <stdlib.h>

#define environ_offset (0x1ef2e0)
#define printf_offset (0x64e10)


char *my_env[10] = 
{
    "account=admin",
};

int main()
{    
    int64_t *environ = printf - printf_offset + environ_offset;
    *environ = my_env;
    printf("account = %s\n", getenv("account"));
}
