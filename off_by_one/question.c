// g_nameAry  = 0x0602040
// g_nameSize = 0x06020e0

void main()
{
  int op;
  
  init();
  while(1){
      menu();
      op = read_input();
      switch(op) {
          case 1:
              create();
              break;

          case 2:
              edit();
              break;

          case 3:
              delete();
              break;

          case 4:
              show();
              break;

          case 5:
              exit(0);
      }
  }

  return 0;
}


void _read(char *_buf, int _len)
{
  read(0, _buf, _len);
  if (iParm2 != 0)
      _buf[_len] = 0;
  return;
}


void read_input()
{
  char buf[24];
  long canary;
  
  canary = *(long *)(in_FS_OFFSET + 0x28);
  _read(buf, 8);
  atoi(buf);

  if (canary != *(long *)(in_FS_OFFSET + 0x28))
    __stack_chk_fail();

  return buf;
}



void init(void)

{
  setvbuf(stdout, 0, 2, 0);
  setvbuf(stdin, 0, 2, 0);
  signal(14, byebye);
  alarm(60);
  return;
}


void menu()
{
  puts("1. Add");
  puts("2. Edit");
  puts("3. Delete");
  puts("4. Show");
  puts("5. Exit");
  puts("Choice: ");
  return;
}


void create()
{
  uint size;
  void *data;

  for(int i = 0; i<=19; i+=1){
      if(!g_nameAry[i])
        break;
  }

  printf("Enter size: ");
  size = read_input();

  data = malloc(size);
  printf("Enter data: ");

  _read(data, size);
  g_nameAry[index]  = data;
  g_nameSize[index] = size;
  puts("Success!");
}


void edit(void)
{
  int index;
  
  printf("Enter index: ");
  index = read_input();

  if (g_nameAry[index])
      puts("Not found!");
  else {
      printf("Enter data: ");
      _read(g_nameAry[index], g_nameSize[index]);
      puts("Success!");
  }
}


void delete(void)
{
  int index;
  
  printf("Enter index: ");
  index = read_input();

  if (g_nameAry[index]) {
      free(g_nameAry[index]);
      g_nameAry[index] = NULL;
      g_nameSize[index] = -1;
      return;
  }
  puts("Not found!");
}


void show(void)
{
  int index;
  
  printf("Enter index: ");
  index = read_input();

  if (g_nameAry[index])
    printf("Data: %s\n",g_nameAry[index]);
  else
      puts("Not found!");
}


void byebye(void)
{
  puts("Bye bye");
}

