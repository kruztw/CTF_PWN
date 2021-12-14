/* 參考: https://dystopia.sg/3kctf21-masterc/
 * tls 的結構為 tcbhead_t 
 *  fs 的開頭可透過 gef> p $fs_base 得知
 *  gef> p/x *((tcbhead_t *)$fs_base)
 */

#include <stdio.h>

int main()
{
    char buf[0x10];
    scanf("%s", buf);
}
