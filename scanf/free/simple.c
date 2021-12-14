// 參考 https://github.com/volticks/CTF-Writeups/tree/main/redpwn%202021/Simultaneity
// scanf 的 scratch buffer size 為 1024
// 只要蓋掉 free_hook, 就能在 free scatch buffer 時跳到任意位址


#include <stdio.h>
#include <stdlib.h>

int main()
{
    long long int addr;

    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);

    printf("puts @ 0x%lx\n", puts);

    printf("addr:");
    scanf("%llx", &addr);

    printf("val:");
    scanf("%llx", addr);

    _exit(0);
}
