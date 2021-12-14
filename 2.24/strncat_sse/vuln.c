// gcc -no-pie -o vuln vuln.c

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct{
	char s1[0x400];
	char s2[0x200];
	char *concatenated_s3;
} packed_strings;

packed_strings strings;

void win(){
	printf("Debug mode activated!\n");
	system("cat flag.txt");
}

void do_strncat(){
	int output_len = 0;
	char *start_s1 = NULL;
	char *start_s2 = NULL;

	printf("First Flag:\n");
	fgets(strings.s1, 0x100, stdin);
	printf("Second Flag:\n");
	fgets(strings.s2, 0x100, stdin);

	output_len = strlen(strings.s1) + strlen(strings.s2);
	char s3[output_len+1];
	strings.concatenated_s3 = s3;

	printf("Going to output %i bytes max!\n", output_len);

	start_s1 = strstr(strings.s1, "hxp{");
	start_s2 = strstr(strings.s2, "hxp{");

	if(!start_s1){
		start_s1 = strings.s1;
	}
	if(!start_s2){
		start_s2 = strings.s2;
	}

	strncat(start_s1, start_s2, SIZE_MAX);
	strcpy(strings.concatenated_s3, start_s1);

	printf("%s\n", strings.concatenated_s3);
}

int main(){
	setbuf(stdout, NULL);
	setbuf(stdin, NULL);
	printf("Welcome to the hxp flag concat protocol server!\n");
	do_strncat();
	return 0;
}
