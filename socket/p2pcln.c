#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>
#include<signal.h>

void handle(int num)
{
    printf("rcv the sig:%d\n", num);
    exit(0);
}

int main(int argc, const char *argv[])
{
    if(argc<=2)
    {
        printf("Usare:%s ip_addr, ip_port", argv[0]);
        return -1;
    }
    if(signal(SIGUSR2, handle)==SIG_ERR)
    {
        perror("signal");
        return -1;
    }

    const char *ip=argv[1];
    int port = atoi(argv[2]);

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

    int ret=0;
    ret=connect(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if(ret<0)
    {
        printf("errno in connect,errno:%d\n", errno);
        return -1;
    }

    int pid = fork();
    if(pid<0)
    {
        printf("error in fork,errno:%d\n", errno);
        return -1;
    }
    if(pid>0)
    {
        //father
        char sndbuf[1024];
        while(fgets(sndbuf, 1024, stdin)!=NULL)
        {
            write(sockfd, sndbuf, 1024);
        }
    }
    else if(pid==0)
    {
        //son
        char rcvbuf[1024]={0};
        while(1)
        {
            int readed=read(sockfd, rcvbuf, 1024);
            if(readed<0)
            {
                printf("read error\n");
                break;
            }
            if(readed==0)
            {
                printf("server exited\n");
                break;
            }
            fputs(rcvbuf, stdout);
            memset(rcvbuf, 0, 1024);
        }
        kill(getppid(), SIGUSR2);
        close(sockfd);
    }


    return 0;
}
