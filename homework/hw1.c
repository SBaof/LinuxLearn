#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/stat.h>
#include<fcntl.h>

void handle(int signum)
{
    if(signum==SIGINT||signum==SIGQUIT)
    {
        printf("正在读取文件，阻塞中...\n");
    }
    if(signum==SIGUSR1)
    {
        sigset_t uset;
        sigemptyset(&uset);
        sigaddset(&uset, SIGINT);
        sigaddset(&uset, SIGQUIT);
        sigprocmask(SIG_UNBLOCK, &uset, NULL);

    }
}
int main(int argc, const char *argv[])
{
    if(signal(SIGINT, handle)==SIG_ERR)
    {
        perror("SIGIT");
        return -1;
    }
    if(signal(SIGQUIT, handle)==SIG_ERR)
    {
        perror("SIGQUIT");
        return -1;
    }
    if(signal(SIGUSR1, handle)==SIG_ERR)
    {
        perror("SIGUSR1");
        return -1;
    }

    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, SIGINT);
    sigaddset(&bset, SIGQUIT);

    sigprocmask(SIG_BLOCK, &bset, NULL);

    int fd = open("./hello", O_RDWR);

    char buff[1024];

    int ret = read(fd, buff, 1024);
    if(ret)
    {
        fputs(buff, stdout);
    }

    printf("read the file finished\n");

    raise(SIGUSR1);

    while(1)
    {
        pause();
    }
    return 0;
}
