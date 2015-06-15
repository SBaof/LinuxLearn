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
    ssize_t nread;
    size_t nleft = maxline;
    char *bufp = buf;

    while(1)
    {
        ret = recv_peek(sockfd, bufp, nleft);
        if(ret < 0)
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
            if(bufp[i]=='\n')
            {
                ret = readn(sockfd, bufp, i+1);
                if(ret != i+1)
                {
                    exit(EXIT_FAILURE);
                }
                return ret;
            }
        }

        if(nread > nleft)
        {
            exit(EXIT_FAILURE);
        }

        nleft -= nread;
        ret = readn(sockfd, bufp, nread);
        if(ret != nread)
        {
            exit(EXIT_FAILURE);
        }

        bufp += nread;
    }
    return -1;
}

void echo_cli(int sock)
{
    char recvbuf[1024] = {0};
    char sendbuf[1024] = {0};

    while(fgets(sendbuf, sizeof(sendbuf), stdin)!=NULL)
    {
        writen(sock, sendbuf, strlen(sendbuf));

        int ret = readline(sock, recvbuf, sizeof(recvbuf));
        if(ret == -1)
        {
            ERR_EXIT("readline");
        }
        else if(ret == 0)
        {
            printf("client closed\n");
            break;
        }

        fputs(recvbuf, stdout);
        memset(recvbuf, 0, sizeof(recvbuf));
        memset(sendbuf, 0, sizeof(sendbuf));
    }
    close(sock);
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

    //print local address
    struct sockaddr_in localaddr;
    socklen_t addrlen = sizeof(localaddr);
    if(getsockname(sockfd, (struct sockaddr *)&localaddr, &addrlen)<0)
    {
        ERR_EXIT("getsockname");
    }
    printf("ip=%s, port=%d\n", inet_ntoa(localaddr.sin_addr), ntohs(localaddr.sin_port));


    echo_cli(sockfd);

    return 0;
}
