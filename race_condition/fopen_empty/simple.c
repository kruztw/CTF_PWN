// gcc simple.c -lcrypto -o simple
// https://elixir.bootlin.com/glibc/glibc-2.29/source/sysdeps/mach/hurd/bits/fcntl.h#L39

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <openssl/md5.h>

char secret[8];
char guess[8];
int rounds;

void my_MD5()
{
	MD5_CTX context;
	MD5_Init(&context);
 	MD5_Update(&context, secret, 8);
 	MD5_Final(secret, &context);
}

int main()
{
	setvbuf(stdin, 0, _IONBF, 0);
	setvbuf(stdout, 0, _IONBF, 0);

	if(access("/tmp/secret", F_OK)){
		int fd1 = open("/tmp/secret", 0);
		int fd2 = open("/dev/urandom", 0);

		read(fd2, secret, 8);
		close(fd2);

		write(fd1, secret, 8);
		close(fd1);
	}

	int fd1 = open("/tmp/secret", 513); // 0x200 => O_ASYNC , 0x1=>O_RDONLY
	read(fd1, secret, 8);

	printf("Rounds:");
	scanf("%d", &rounds);
	while(rounds-- > 0)
		my_MD5();
	write(fd1, secret, 8);
	close(fd1);

	printf("Guess secret: ");
	read(0, guess, 8);

	if(!memcmp(guess, secret, 8))
		system("/bin/sh");
	else
		puts("Nope!");

	return 0;
}