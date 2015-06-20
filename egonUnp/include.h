#ifndef __include_h
#define __include_h

#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<errno.h>
#include<fcntl.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>

#define ERR_EXIT(str) \
    do \
    { \
        perror(str); \
        exit(EXIT_FAILURE); \
    }while(0)


#define SA    struct sockaddr
#define LISTENQ    1024

#define MAXLINE    4096
#define BUFFSIZE   8192

#define SERV_PORT    9877
#define SERV_PORT_STR    "9877"

#define bzero(ptr, n)    memset(ptr, 0, n)

void str_echo(int sockfd);
ssize_t readn(int fd, void *vptr, size_t n);
ssize_t writen(int fd, void *vptr, size_t n);
ssize_t readline(int fd, void *vptr, size_t maxline);

#endif //__include_h
