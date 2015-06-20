#include "include.h"


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

void str_echo(int sockfd)
{
    ssize_t n;
    char buf[MAXLINE];
again:
    while((n=read(sockfd, buf, MAXLINE))>0)
    {
        writen(sockfd, buf, n);
	fputs(buf, stdout);
	bzero(buf, sizeof(buf));
    }
    if(n<0 && errno == EINTR)
    {
        goto again;
    }
    else if(n<0)
    {
        ERR_EXIT("str_echo error: readn error");
    }
}

int main(int argc, const char *argv[])
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in cliaddr, servaddr;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);

    bind(listenfd, (SA*)&servaddr, sizeof(servaddr));

    listen(listenfd, LISTENQ);

    for(;;)
    {
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (SA*)&cliaddr, &clilen);

        if((childpid = fork())==0)
        {
            close(listenfd);
            str_echo(connfd);
            exit(0);
        }
        close(connfd);
    }


    return 0;
}


