#include <stdio.h>
#include <string.h>

int main() {
    char *buf    = "GET \n 12345678 HTTP/1.1\r\n";
    char resource[100] = {};

    char *space1 = strchr(buf, ' ');
    char *part2  = space1 + 1;
    char *space2 = strchr(part2, ' ');

    printf("space2 - part2 + 1 = %ld\n", space2 - part2 + 1); // 2
    sscanf(buf, "GET %s HTTP/1.1\r\n", resource);
    printf("resource = %s\n", resource);                      // 12345678
}
