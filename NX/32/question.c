void main()
{
  char local_c [8];
  
  read(0,local_c,100);
  close(0);
  close(1);
  close(2);
  return 0;
}