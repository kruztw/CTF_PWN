/* libc: 2.31 */
#include <stdio.h>
#include <stdlib.h>

void backdoor()
{
    system("/bin/sh");
}

int main()
{
    int64_t vuln[1];
    printf("backdoor @ %ld\n", backdoor);
    for(int i = 0; i<4; i++)
        scanf("%ld", &vuln[i]);
}
