/*
 * gcc simple.c -o simple
 * libc: 2-31
 * assert 會呼叫 asprintf, 其中 __proogram 位址由 program_invocation_short_name 決定
 * 但輸出是在 stderr, 所以可能要自行 dup stderr 到 stdout
 */

#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

#define program_invocation_short_name_offset (0x0000000001ec440)
#define open_offset (0x000000000110e50)

char flag[0x20];

int main()
{
    int fd = open("flag", O_RDONLY);
    void *target = open - open_offset + program_invocation_short_name_offset;

    if (fd > 0)
        read(fd, flag, sizeof(flag));

    *(long long int *)target = flag;
    assert(0);
}
