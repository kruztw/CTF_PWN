#include <errno.h>
#include <linux/filter.h>
#include <linux/seccomp.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

static void perror_exit(char *msg)
{
        perror(msg);
        exit(1);
}

static int seccomp(unsigned int op, unsigned int flags, void *args)
{
        errno = 0;
        return syscall(SYS_seccomp, op, flags, args);
}

int main(int argc, char *argv[])
{
        unsigned short num_insns;
        struct sock_filter *insns;
        struct sock_fprog prog;

        scanf("%hu", &num_insns);
        insns = calloc(num_insns, sizeof(*insns));
        for (int i = 0; i < num_insns; i++)
            scanf(" %hx %hhx %hhx %x", &insns[i].code, &insns[i].jt, &insns[i].jf, &insns[i].k);

        prog.len = num_insns;
        prog.filter = insns;

        if (seccomp(SECCOMP_SET_MODE_FILTER, 0, &prog))
            perror_exit("seccomp");

        execv(argv[1], &argv[1]);
}
