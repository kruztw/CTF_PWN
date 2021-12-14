/*
 * 根據 man scanf
 * The format string consists of a sequence of directives which describe how to process the sequence of input characters.  If processing of a directive fails, no  further  input  is  read,  and
   scanf()  returns.   A  "failure"  can be either of the following: input failure, meaning that input characters were unavailable, or matching failure, meaning that the input was inappropriate
 *
 * 也就是說, 輸入 1x => a = 1
 *           輸入 x  => a = 0xfaceb00c
 *
 * %d %u %x 等等, 都一樣
 */

#include <stdio.h>

int main()
{
    int a = 0xfaceb00c;
    scanf("%d", &a);
    printf("%x", a);
}
