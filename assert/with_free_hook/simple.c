/*
 * 先把 free_hook 寫入 system
 * 再透過 bof 寫入 \x00... + /bin/sh;...
 * \x00 目的是要把 *environ 蓋成 '\x00'
 * assert 會呼叫 free, 參數在 stack 上 (確認方式: 用 gdb 看 asprint 呼叫位址)
 * 所以 /bin/sh;... 就相當於執行 system(...h;/bin/sh;...)
 * 另外發現二個有趣現象
 * 1. scanf("%s") 可以讀 '\x00' 
 * 2. system 最後是呼叫 clone (參見 system 資料夾), 失敗原因通常是 *environ 指到錯誤位址 (err code = 0xe), 反而不是暫存器的問題 (看來之前都搞錯了)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <assert.h>

int main()
{
    char buf[0x10];
    void *ptr;

    setvbuf(stdout, 0, 2, 0);

    printf("printf @ %p\n", &printf);
    printf("address:");

    scanf("%ld", (int64_t *)&ptr);
    printf("value:");
    read(0, ptr, sizeof(ptr));

    scanf("%s", buf);
    assert(0);
}
