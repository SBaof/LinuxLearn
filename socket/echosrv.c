#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

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
/*
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
    while(1)
    {
        int ret = recv(sockfd, buf, len, MSG_PEEK);
        if(ret == -1 && errno == EINTR)
        {
            continue;
        }
        return ret;
    }
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
    int ret;
    int nread;
    char *bufp = buf;
    int nleft = maxline;
    while(1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret<0)
        {
            return ret;
        }
        else if(ret == 0)
        {
            return ret;
        }

        nread = ret;
        int i=0;
        for(i=0; i<nread; i++)
        {
            if(bufp[i]=='\n')//缓冲区有\n
            {
                ret = readn(sockfd, bufp, i+1);//读走数据
                if(ret != i+1)
                {
                    exit(EXIT_FAILURE);
                }
                return ret;//有\n就返回，并返回读走的数据
            }
        }

        if(nread > nleft)//如果读走的数大于一行最大数则异常处理
        {
            exit(EXIT_FAILURE);
        }

        nleft -= nread;//若缓冲区没有\n，把剩余的数据读走
        ret = readn(sockfd, bufp, nread);
        if(ret != nread)
        {
            exit(EXIT_FAILURE);
        }
        bufp += nread;//bufp指针后移后，再接着偷看缓冲区数据recv_peek,直到遇到\n
    }
    return -1;
}*/

ssize_t readline(int sockfd, void *vptr, size_t maxlen)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for(n=1; n<maxlen; n++)
    {
        again:
        if((rc=read(sockfd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if(c == '\n')
            {
                break;//new line is stored, like fgets();
            }
        }
        else if(rc == 0)
        {
            *ptr = 0;
            return (n-1);//EOF, n-1 bytes were read
        }
        else
        {
            if(errno == EINTR)
            {
                goto again;
            }
            return -1; //error, errno set by read
        }
    }

    *ptr = 0; //null terminate like fgets()
    return n;
}


/*
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

        n = ntohs(recvbuf.len);
        ret = readline(conn, &recvbuf.buf, n);
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
    }
}
*/

void do_service(int conn)
{
    char recvbuf[1024];
    while(1)
    {
        //printf("in do_service\n");
        memset(recvbuf, 0, sizeof(recvbuf));
        int ret = readline(conn, recvbuf, 1024);
        //printf("ret=%d\n", ret);
        if(ret == -1)
        {
            ERR_EXIT("readline");
        }
        if(ret == 0)
        {
            printf("client closed\n");
            break;
        }

        //printf("%s\n", recvbuf);
        fputs(recvbuf, stdout);
        writen(conn, recvbuf, sizeof(recvbuf));
    }
}

int main(int argc, const char *argv[])
{
    if(argc<=2)
    {
        printf("Usage:%s ip_addr, ip_port\n", argv[0]);
        return -1;
    }

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

    int on;
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

        printf("connected client:ip:%s, port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));

        pid = fork();
        if(pid==-1)
        {
            ERR_EXIT("fork");
        }
        if(pid == 0)
        {
            close(sockfd);
            do_service(conn);
            //exit(EXIT_SUCCESS);
        }
        else
        {
            close(conn);
        }
    }
    return 0;
}
