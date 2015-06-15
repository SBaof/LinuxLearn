#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>

int main(int argc, const char *argv[])
{
    if(argc<=2)
    {
        printf("Usare:%s ip_addr, ip_port", argv[0]);
        return -1;
    }

    const char *ip=argv[1];
    int port = atoi(argv[2]);

    int i=0;
    for(i=0; i<10; i++)
    {
    int sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd<0)
    {
        printf("error in socket,errno:%d\n", errno);
        return -1;
    }

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);

    {int ret=0;
    ret=connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if(ret<0)
    {
        printf("errno in connect,errno:%d\n", errno);
        return -1;
    }

    char rcvbuff[1024];
    char sndbuff[1024];
    while(fgets(sndbuff, 1024, stdin)!=NULL)
    {
     //   close(sockfd);
//        sprintf(sndbuff, "i:%d\n", i+1);
//        strcpy(sndbuff, "qqqq");
        write(sockfd, sndbuff, 1024);
        read(sockfd, rcvbuff, 1024);
        fputs(rcvbuff, stdout);
        memset(rcvbuff, 0, 1024);
        memset(sndbuff, 0, 1024);
    }

    }
    //close(conn);
    close(sockfd);
    }

    return 0;
}
