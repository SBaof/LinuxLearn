#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<signal.h>
#include<sys/wait.h>

#define ERR_EXIT(m) \
        do \
        { \
            perror(m); \
            exit(EXIT_FAILURE); \
        }while(0)

typedef struct packet_
{
    int len;
    char buf[1024];
}packet;

void handle(int signum)
{
    if(signum == SIGCHLD)
    {
        int mypid;
        while((mypid=waitpid(-1, NULL, WNOHANG)) > 0)
        {
            printf("get the child, pid: %d\n", mypid);
        }
    }
}

ssize_t readn(int fd, void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nread;
    char *bufp = buf;

    while( nleft > 0)
    {
        if((nread = read(fd, bufp, nleft))<0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            return -1;
        }
        else if(nread == 0)
        {
            return count - nread;
        }

        bufp += nread;
        nleft -= nread;
    }
    return count;
}

ssize_t writen(int fd, const void *buf, size_t count)
{
    size_t nleft = count;
    ssize_t nwritten;
    const char *bufp = buf;

    while(nleft > 0)
    {
        if((nwritten = write(fd, bufp, nleft))<0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            return -1;
        }
        else if(nwritten ==0 )
        {
            continue;
        }

        bufp += nwritten;
        nleft -= nwritten;
    }

    return count;
}

void do_service(int conn)
{
    packet recvbuf;
    int n;
    while(1)
    {
        memset(&recvbuf, 0, sizeof(recvbuf));

        int ret = readn(conn, &recvbuf.len, 4);
        if(ret == -1)
        {
            ERR_EXIT("readn");
        }
        else if(ret<4)
        {
            printf("client closed\n");
            break;
        }

        n = ntohl(recvbuf.len);
       // printf("recvbuf.len=%d\n", n);
        ret = readn(conn, recvbuf.buf, n);
        if(ret == -1)
        {
            ERR_EXIT("readn");
        }
        else if(ret < n)
        {
            printf("client closed\n");
            break;
        }

        fputs(recvbuf.buf, stdout);

        writen(conn, &recvbuf, 4+n);

        //printf("recvbuf.len=%d\n", recvbuf.len);
    }
}

int main(int argc, const char *argv[])
{
    if(argc<=2)
    {
        printf("Usage:%s ip_addr, ip_port\n", argv[0]);
        return -1;
    }

    signal(SIGCHLD, handle);

    const char *ip = argv[1];
    int port = atoi(argv[2]);

    int sockfd;
    if((sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
    {
        ERR_EXIT("socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int on=1;
    if(setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
    {
        ERR_EXIT("setsockopt");
    }

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))<0)
    {
        ERR_EXIT("bind");
    }

    if(listen(sockfd, SOMAXCONN)<0)
    {
        ERR_EXIT("listen");
    }

    struct sockaddr_in peeraddr;
    socklen_t peerlen = sizeof(peeraddr);
    pid_t pid;
    int conn;
    while(1)
    {
        if((conn = accept(sockfd, (struct sockaddr *)&peeraddr, &peerlen))<0)
        {
            ERR_EXIT("accept");
        }

        printf("ip:%s, port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        pid = fork();
        if(pid==-1)
        {
            ERR_EXIT("fork");
        }
        if(pid == 0)
        {
            close(sockfd);
            do_service(conn);
            exit(EXIT_SUCCESS);
        }
        else
        {
            close(conn);
        }
    }
    return 0;
}
