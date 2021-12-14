// g++ simple.cpp -o chall -ggdb

#include <stdio.h>
#include <stdint.h>
#include <sys/syscall.h>

int main()
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);
    uint64_t rax, rdi, rsi, rdx, r10, r8, r9;

    while(1){
        printf("rax = ");
        scanf("%lu", &rax);
        printf("rdi = ");
        scanf("%lu", &rdi);
        printf("rsi = ");
        scanf("%lu", &rsi);
        printf("rdx = ");
        scanf("%lu", &rdx);
        printf("r10 = ");
        scanf("%lu", &r10);
        printf("r8 = ");
        scanf("%lu", &r8);
        printf("r9 = ");
        scanf("%lu", &r9);

        asm volatile ("movq %0, %%rdi":: "a"(rdi));
        asm volatile ("movq %0, %%rsi":: "a"(rsi));
        asm volatile ("movq %0, %%rdx":: "a"(rdx));
        asm volatile ("movq %0, %%r10":: "a"(r10));
        asm volatile ("movq %0, %%r8":: "a"(r8));
        asm volatile ("movq %0, %%r9":: "a"(r9));
        asm volatile ("movq %0, %%rax":: "a"(rax));
        asm volatile ("syscall");
        asm volatile ("movq %%rax, %0": "=a"(rax));
        printf("rax = 0x%lx\n", rax);
    }
}