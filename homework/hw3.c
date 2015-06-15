#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<wait.h>
#include<signal.h>

int pidArray[10]={ 0 };
void myhandle(int signum, siginfo_t *info, void *p)
{
    printf("rcv signal:%d\n", signum);
    if(signum==SIGRTMIN+1)
    {
        //son1 get the message
        printf("son1 get the message,val:%d\n", info->si_int);
        pidArray[1]=info->si_int;
    }
    if(signum==SIGRTMIN+2)
    {
        //son2 get the message
        printf("son2 get the message,val:%d\n", info->si_int);
        pidArray[3]=info->si_int;
    }
    if(signum==SIGRTMIN+3)
    {
        //son2 get the message
        printf("father get the message,val:%d\n", info->si_int);
        printf("pid[0]:%d, pid[1]:%d\n", pidArray[0], pidArray[1]);
//        pidArray[4]=info->si_int;
    }
}


int main(int argc, const char *argv[])
{
    int i=0;
    pid_t pid;
    struct sigaction act;
    act.sa_sigaction = myhandle;
    act.sa_flags = SA_SIGINFO;

    if((sigaction(SIGRTMIN+1, &act, NULL))!=0)
    {
        perror("sigaction");
        exit(0);
    }
    if((sigaction(SIGRTMIN+2, &act, NULL))!=0)
    {
        perror("sigaction");
        exit(0);
    }
    if((sigaction(SIGRTMIN+3, &act, NULL))!=0)
    {
        perror("sigaction");
        exit(0);
    }

    for(i=0; i<2; i++)
    {
        pidArray[i] = pid = fork();
        //printf("pidArray[%d]:%d\n", i, pidArray[i]);
        if(pid<0)
        {
            printf("error in fork, errno:%d\n", errno);
            return -1;
        }
        if(pid ==0 )
        {
            break;
        }
        if(pid>0)
        {
            ;
        }
    }
    if(pid>0)
    {
        union sigval val;
        val.sival_int = pidArray[1];
        int res = sigqueue(pidArray[0], SIGRTMIN+1, val);
        if(res!=0)
        {
            printf("error in sigqueue,errorno:%d\n", errno);
            return -1;
        }

        int mypid=0;
        while((mypid=waitpid(-1, NULL, 0))>0)
        {
            printf("son %d exited\n", mypid);
        }

        printf("in father, pid:%d\n", getpid());
    }

    if(pid==0&&i==0)
    {
        printf("son1 runs\n");
        printf("son1 sleep\n");
        //sleep(5);
        //printf("son1, pid:%d, father pid:%d\n", getpid(), getppid());
        printf("son1 get the pid of son2\n");

        union sigval val;
        val.sival_int = getpid()*2;
        printf("son1(%d) send message to son2\n", getpid());
        int res = sigqueue(pidArray[1], SIGRTMIN+2, val);
        if(res!=0)
        {
            printf("error in sigqueue,errorno:%d\n", errno);
            return -1;
        }
        exit(0);
    }
    if(pid==0&&i==1)
    {
        sleep(10);
        printf("son2 aweak, pid:%d, send mess to father\n", getpid());

        union sigval val;
        val.sival_int = pidArray[3];
        printf("son2(%d) send message to father\n", getpid());
        int res = sigqueue(getppid(), SIGRTMIN+3, val);
        if(res!=0)
        {
            printf("error in sigqueue,errorno:%d\n", errno);
            return -1;
        }
        exit(0);
    }

    return 0;
}
