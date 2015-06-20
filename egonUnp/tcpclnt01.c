#include "include.h"

ssize_t readline(int fd, void *vptr, size_t maxline)
{
    ssize_t n, rc;
    char c, *ptr;

    ptr = vptr;
    for(n=1; n<maxline; n++)
    {
        again:
        if((rc = read(fd, &c, 1)) == 1)
        {
            *ptr++ = c;
            if(c=='\n')
            {
                break;
            }
        }
        else if(rc == 0)
        {
            *ptr = 0;
            return (n-1);
        }
        else
        {
            if(errno == EINTR)
            {
                goto again;
            }
            return -1;
        }
    }
    *ptr = 0;
    return (n);
}

ssize_t writen(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nwritten;
    char *ptr;

    ptr = vptr;
    nleft = n;

    while(nleft>0)
    {
        if((nwritten = write(fd, ptr, nleft)) <=0)
        {
            if(nwritten<0 && errno == EINTR)
            {
                nwritten = 0;
            }
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return (n);
}

ssize_t readn(int fd, void *vptr, size_t n)
{
    size_t nleft;
    ssize_t nread;
    char *ptr;

    ptr = vptr;
    nleft = n;
    while(nleft>0)
    {
        if((nread = read(fd, ptr, nleft)) < 0)
        {
            if(errno == EINTR)
            {
                continue;
            }
            else
            {
                return -1;
            }
        }
        else if(nread == 0)
        {
            break;
        }

        nleft -= nread;
        ptr += nread;
    }
    return (n-nleft);
}

void str_cli(int sockfd)
{
    char sendline[MAXLINE], recvline[MAXLINE];

    while(fgets(sendline, MAXLINE, stdin)!=NULL)
    {
        writen(sockfd, sendline, strlen(sendline));
    }
    if(readline(sockfd, recvline, MAXLINE)==0)
    {
        ERR_EXIT("str_cli:server terminated prematurely");
    }
    fputs(recvline, stdout);
    bzero(recvline, sizeof(recvline));
    bzero(sendline, sizeof(sendline));
}

int main(int argc, const char *argv[])
{
    int sockfd;
    struct sockaddr_in servaddr;

    if(argc!=2)
    {
        printf("Usage: %s ip_address\n", argv[0]);
        return -1;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(SERV_PORT);
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

    str_cli(sockfd);

    return 0;
}
