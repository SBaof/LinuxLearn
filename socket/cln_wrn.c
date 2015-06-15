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

void echo_cln(int sockfd)
{
    packet sendbuf;
    packet recvbuf;
    memset(&sendbuf, 0, sizeof(sendbuf));
    memset(&recvbuf, 0, sizeof(recvbuf));

    int n;
    while(fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin)!=NULL)
    {
        n = sizeof(sendbuf.buf);
        sendbuf.len = htonl(n);
        writen(sockfd, &sendbuf, 4+n);

        int ret = readn(sockfd, &recvbuf.len, 4);
//        printf("ret:%d\n", ret);
        if(ret == -1)
        {
            ERR_EXIT("readn");
        }
        else if(ret<4)
        {
            printf("client closed\n");
            //break;
            exit(EXIT_FAILURE);
        }

        readn(sockfd, recvbuf.buf, n);

        printf("get: ");
        fputs(recvbuf.buf, stdout);
        memset(&sendbuf, 0, sizeof(sendbuf));
        memset(&recvbuf, 0, sizeof(recvbuf));
    }
    close(sockfd);
}

int main(int argc, const char *argv[])
{
    if(argc<2)
    {
        printf("Uage:%s ip_address, ip_port\n", argv[0]);
        return -1;
    }

    int port = atoi(argv[2]);
    const char *ip = argv[1];

    int sockfd;

    if((sockfd=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))<0)
    {
        ERR_EXIT("socket");
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    int ret = connect(sockfd, (struct sockaddr *)&addr, sizeof(addr));
    if(ret < 0)
    {
        ERR_EXIT("connect");
    }


    echo_cln(sockfd);
    close(sockfd);
    return 0;
}
