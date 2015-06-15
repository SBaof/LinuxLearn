#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<time.h>

pthread_t tid;
sigset_t set;

void myfun()
{
    printf("hello\n");
}

static void *mythread(void *p)
{
    int signum;
    while(1)
    {
        sigwait(&set, &signum);
        if(SIGUSR1==signum)
        {
            myfun();
        }
        if(SIGUSR2==signum);
        {
            printf("I will sleep for 2 second then exit\n");
            sleep(2);
            break;
        }
    }
}

int main(int argc, const char *argv[])
{
    char temp;
    void *status;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigprocmask(SIG_SETMASK, &set, NULL);
    pthread_creat(&tid, NULL, mythread, NULL);
    while(1)
    {
        printf(":");
        scanf("%c",&temp);
        if('a'==temp)
        {
            pthread_kill(tid, SIGUSR1);
        }
        else if('q'==temp)
        {
            pthread_kill(tid, SIGUSR2);
            pthread_join(tid, &status);
            printf("finish\n");
            break;
        }
        else
        {
            continue;
        }
    }
    return 0;
}
