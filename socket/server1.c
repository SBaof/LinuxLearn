//解决僵尸进程的问题

#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<arpa/inet.h>
#include<signal.h>
#include<sys/wait.h>

void handle_chld1(int signum)
{
    wait(NULL);
}

void handle_chld2(int signum)
{
    int mypid;
    while((mypid=waitpid(-1, NULL, WNOHANG))>0)
    {
        ;
    }
}


int main(int argc, const char *argv[])
{
    if(argc<=2)
    {
        printf("Usage:%s ip_addr, ip_port\n", argv[0]);
        return -1;
    }

//    signal(SIGCHLD, SIG_IGN);
    signal(SIGCHLD, handle_chld2);

    int sockfd;
    int port = atoi(argv[2]);
    const char *ip = argv[1];

    sockfd=socket(AF_INET, SOCK_STREAM, 0);
    if(socket<0)
    {
        printf("error in socket,errno:%d\n", errno);
        return -1;
    }

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

    while(1)
    {
        int conn;
        conn = accept(sockfd, (struct sockaddr*)&client, &len);
        if(conn == -1)
        {
            printf("error in accept,errno:%d\n", errno);
            return -1;
        }

        int pid=0;
        pid=fork();
        char rcvbuf[1024];
        if(pid==0)
        {
            close(sockfd);
            while(1)
            {
                int red;
                red=read(conn, rcvbuf, 1024);
                if(red==0)
                {
                    printf("client closed\n");
                    close(conn);
                    exit(0);
                }
                fputs(rcvbuf, stdout);
                write(conn, rcvbuf, 1024);
                memset(rcvbuf, 0, 1024);
            }
        }
        else if(pid>0)
        {
            close(conn);
        }
    }
    //close(conn);
    close(sockfd);
    return 0;
}
