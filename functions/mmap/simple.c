/*
    MAP_FIXED : 可以取代已經存在的段
    MAP_FIXED_NOREPLACE : 不能取代, 可用來猜位址 

    參考: https://www.willsroot.io/2021/03/zer0pts-ctf-2021-nasm-kit-writeup.html
*/

#include <sys/mman.h>
#include <stdint.h>

int main()
{
    mmap(((int64_t)main+0x1000) & 0xfffffffffffff000, 0x1000, 7,  MAP_PRIVATE | MAP_ANON | MAP_FIXED_NOREPLACE, -1, 0);
    mmap(((int64_t)main+0x1000) & 0xfffffffffffff000, 0x1000, 7,  MAP_PRIVATE | MAP_ANON | MAP_FIXED, -1, 0);
}
