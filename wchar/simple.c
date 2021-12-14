#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <unistd.h>
#include <locale.h>

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8");
    setvbuf(stdin, 0, 2, 0);
    setvbuf(stdout, 0, 2, 0);
    close(2);
    wprintf(L"close @ %p\n", close);
    for(int i = 0; i<1024; i++){
      wchar_t buf;
      wscanf(L"%lc", &buf);
      wprintf(L"%lc\n", buf);
      fputwc(buf, stderr);
    }
    return 0;
}
