#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <unistd.h>

#define SIZE 0x100

int main(int argc, char **argv) {
  unsigned char out[SIZE] = {};
  unsigned char in[SIZE] = {};
  setlocale(0, "en_US.UTF-8");

  if (argv[1][0] == '1') {
    read(0, in, SIZE);
    mbstowcs((wchar_t*)out, in, SIZE);
    write(1, out, sizeof(out));
  } else {
    read(0, in, SIZE);
    wcstombs(out, (wchar_t*)in, SIZE);
    write(1, out, sizeof(out));
  }
  return 0;
}