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
        printf("Usage:%s ip_addr, ip_port\n", argv[0]);
        return -1;
    }

    int sockfd;
    int port = atoi(argv[2]);
    const char *ip = argv[1];

    if(signal(SIGUSR1, handle)==SIG_ERR)
    {
        perror("signal");
        exit(0);
    }

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(socket<0)
    {
        printf("error in socket,errno:%d\n", errno);
        return -1;
    }

    int  reuse = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

    struct sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &sockaddr.sin_addr);
    int ret=0;
    ret = bind(sockfd, (struct sockaddr*)&sockaddr, sizeof(sockaddr));
    if(ret!=0)
    {
        printf("error in bind,errno:%d\n", errno);
        return -1;
    }

    ret = listen(sockfd, SOMAXCONN);
    if(ret<0)
    {
        printf("error in listen,errno:%d\n", errno);
        return -1;
    }

    struct sockaddr_in client;
    socklen_t len = sizeof(client);

    int conn;
    conn = accept(sockfd, (struct sockaddr*)&client, &len);
    if(conn == -1)
    {
        printf("error in accept,errno:%d\n", errno);
        return -1;
    }

    int pid = fork();
    if(pid<0)
    {
        printf("error in fork\n");
        return -1;
    }
    if(pid>0)
    {
        //father
        char rcvbuf[1024]={0};
        while(1)
        {
            int readed=read(conn, rcvbuf, 1024);
            if(readed==0)
            {
                printf("client exited\n");
//                exit(0);
                break;
            }
            fputs(rcvbuf, stdout);
            memset(rcvbuf, 0, 1024);
        }
        kill(pid, SIGUSR1);
    }
    else if(pid==0)
    {
        //son
        char sndbuf[1024]={0};
        while(fgets(sndbuf, 1024, stdin)!=NULL)
        {
            write(conn, sndbuf, 1024);
        }
    }

    close(conn);
    close(sockfd);
    return 0;
}
