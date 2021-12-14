void main()
{
    undefined auVar3 [16];
    char feedback[1024];
  	char buf[1032];
  
  	local_838 = 0;
  	name = &local_838;
  	alarm(10);
  	setlinebuf(stdout);
  	setlinebuf(stderr);
  	memset(feedback, 0, 0x800);
  	puts("Welcome to Monoid Operator!!");

    while( true ) {
    	local_844 = 0;
    	puts("What operator do you choose?");
    	op = '\0';
    	tmp = scanf("%c", &op);
    	if (tmp == 0 || tmp == -1)
        	_exit(1);

       	if (op == 'q') {
       	    write(1,"Before end, please submit feed back!\nWhat is your name?\n",0x38);
            len = read(0, name, 7);
          	name[len] = 0;
          	printf("Hi, %s.\nPlease write your feed back!\n",name);
          	len = read(0,feedback, 0x3ff);
          	feedback[len] = '\0'; // off-by-one
          	if (strchr(feedback, 'n'))
             	  _exit(1);
          				
          	sprintf(buf, feedback);
          	return 0;
      	}

        if (op == '+' || op == '*' || op == '^'){
        	fwrite("No such operator is supported.Sorry.\n",1,0x25,stderr);
        	back
        }
      			
      	puts("How many integers do you input?");
      	input_num = 0;
      	scanf("%d", &input_num);
      	puts("Input integers.");
      	ptr = malloc(input_num*8);
      	if (ptr){
      		puts("Internal Error Occurs.");
      		back;
    	}
			
		for(int i = 0; i<input_num; i++)
      		scanf("%lu", ptr[i]);
    		
    	total = 0; // rbp-0x28
    	if (op == '+') {
    		for(int i = 0; i<input_num; i++){
      			value = ptr[i];
        		tmp = CARRY8(total,value);
        		total += value;
        		if (tmp) goto overflow;
      		}
    	}
    	else if (op == '^') {
    		for(int i = 0; i<input_num; i++)
          		total ^= ptr[i];
      	}
      	else if (op == '*') {
          	total = 1;
          	for(int i = 0; i<input_num; i++){
            		lVar2 = ptr[i];
            		auVar3 = SEXT816(total);
            		total *= lVar2;
            		lVar2 = SUB168(SEXT816(lVar2) * auVar3 >> 0x40,0);
            		if (lVar2 != 0 && lVar2 != -1) goto overflow;
        	}
    	}
    	
    	printf("The answer is %lu.\n",total);
    	fflush(stdout);
    	free(ptr);
    }
	overflow:
  		fwrite("Overflow is detected.\n",1,0x16,stderr);
  		free(ptr);
}