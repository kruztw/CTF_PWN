#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>
#include <signal.h>

#define 0x200 0x200

char text[0x200];
char filepath[32];
FILE *fp;
int init;

void show_menu(void)
{
  puts("=-=-=-=-=-=-=-=-=-=-=-=");
  puts("1. Open the book");
  puts("2. Read the book");
  puts("3. Revise the contents");
  puts("4. Close the book");
  puts("=-=-=-=-=-=-=-=-=-=-=-=");
  printf("> ");
}

void error(const char *s, char *errstring)
{
  const char *colon;
  char *errmsg;
  
  if (s == NULL || *s == '\0') {
    s = colon = "";
  } else {
    colon = ": ";
  }

  errmsg = (char*)malloc(strlen(s) + strlen(colon) + strlen(errstring) + 2);
  snprintf(errmsg, malloc_usable_size(errmsg), "%s%s%s\n", s, colon, errstring);
  fprintf(stderr, errmsg); // fmt
  free(errmsg);
}

int readline(char *buf, int size)
{
  int readSize = read(0, buf, size);
  
  if (buf[readSize - 1] == '\n') readSize--;
  if (readSize == 0) {
    error("read", "Received nothing");
    exit(1);
  }
  
  buf[readSize] = '\x00';

  return readSize;
}

int read_int(void)
{
  char buf[8];
  readline(buf, 7);
  return atoi(buf);
}

void grimoire_open(void)
{
  if (fp) {
    puts("The desk is occupied with another book.");
    return;
  }
  
  fp = fopen(filepath, "r");
  if (fp == NULL) {
    error(filepath, "No such file or directory");
    puts("You lost the grimoire...");
    return;
  }
  
  puts("You opened the forbidden grimoire.");
}

void grimoire_close(void)
{
  if (fp == NULL) {
    puts("You don't even open the book.");
    return;
  }
  
  puts("You closed the book before something bad happens.");
  fclose(fp);
  fp = NULL;
  init = 0;
}

void grimoire_read(void)
{
  unsigned short size;
  char buffer[0x200];
  
  if (fp == NULL) {
    puts("You don't even open the book.");
    return;
  }

  if (init == 0) {
    init = 1;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    memset(buffer, 0, 0x200);
    if (fread(buffer, sizeof(char), size, fp) == 0) {
      error("fread", "Empty file stream");
      return;
    } else {
      memcpy(text, buffer, 0x200);
    }
  }
  
  puts("*----------------------------------------*");
  printf("%s", text); // may leak
  puts("*----------------------------------------*");
}

void grimoire_edit(void)
{
  char buffer[0x200];
  unsigned short offset, size;
  
  if (init == 0) {
    puts("You must read the book before editing it.");
    return;
  }

  printf("Offset: ");
  offset = read_int();
  if (offset > 0x200) {
    puts("You can't add a new page.");
    return;
  }
  
  printf("Text: ");
  size = read(0, buffer, 0x200);
  
  if (size == 0) {
    error("read", "Received nothing");
    exit(1);
  }
  if (buffer[size - 1] == '\n') size--;
  buffer[size] = 0; // off-by-one
  
  memcpy(text + offset, buffer, size); // bof
}

void handler(int sig)
{
  if (sig == SIGALRM) {
    puts("\nIt's time to say goodbye.");
    exit(1);
  }
}

void initialize(void)
{
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  
  signal(SIGALRM, handler);
  alarm(60);
  
  strcpy(filepath, "grimoire.txt");
  init = 0;
}

int main(void)
{
  int choice;

  initialize();
  
  while(1) {
    show_menu();
    choice = read_int();
    
    switch(choice) {
    case 1:
      grimoire_open();
      break;
    case 2:
      grimoire_read();
      break;
    case 3:
      grimoire_edit();
      break;
    case 4:
      grimoire_close();
      break;
    default:
      puts("Invalid choice.");
      break;
    }
  }
}
