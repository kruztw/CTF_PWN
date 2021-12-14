#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <x86intrin.h>

#define PAGE_SIZE 4096
#define FLAG_SIZE 28
#define DEBUG 0

const char pages[] = { [0 ... 256 * PAGE_SIZE] = 1 };
static char secret[64];

int victim_function (size_t x)
{
    int y = 0;
    if (x < 100) {
        y = pages[secret[x] * PAGE_SIZE];
    }
    return y;
}

int main ()
{
    FILE *fp = fopen("./flag.txt", "r");
    if (fp == NULL) {
        perror("Error while opening the file.\n");
        exit(EXIT_FAILURE);
    }

    fgets(secret, FLAG_SIZE, fp);

    char buffer[64];
    size_t x;
    int i;
    int nb_read;

    __asm__ volatile("lea %0, %%rax\n"
                     "movb (%%rax), %%al\n" ::"m"(secret)
                     :);

    while (scanf("%ld", &x) == 1) {
        memset (buffer, '\0', 64);
        x = victim_function (x);
    }
    return 0;
}

