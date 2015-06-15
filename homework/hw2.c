#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>
#include<wait.h>

int main(int argc, const char *argv[])
{
    int pid;
    int pronum;
    int loop;
    int i=0, j=0;
    printf("进程数：");
    scanf("%d", &pronum);
    printf("圈数：");
    scanf("%d", &loop);

    for(i=0; i<pronum; i++)
    {
        pid=fork();
        {
            if(pid<0)
            {
                perror("fork");
                return -1;
            }
            else if(pid==0)
            {
                for(j=0; j<loop; j++)
                {
                    printf("process %d runs %d loop, father:%d\n", i+1, j+1, getppid());
                }
                printf("\n");
                //exit(i+1);
                exit(0);
            }
        /*else
        {
            wait(NULL);
        }*/
            }
    }

 //   int status;
/*    do
    {
        int ret;
        ret = wait(&status);
        if(ret == -1)
        {
            if(errno==EINTR)
            {
                continue;
            }
            break;
        }
        if(WIFEXITED(status))
        {
            printf("%d exited, status=%d\n",ret, WEXITSTATUS(status));
        }
    }while(!WIFEXITED(status)&&i<=pronum);
    */
/*    while(1)
    {
        int ret = wait(NULL);
        if(ret==-1)
        {
            if(errno==EINTR)
            {
                continue;
            }
            break;
        }
    }
*/

    int mypid=0;
    //while((mypid=waitpid(-1, NULL, 0))>0)
    while((mypid=waitpid(-1, NULL, WNOHANG))>0)
    {
        printf("pid:%d exited\n", mypid);
    }

    printf("I:%d All son exited\n", getpid());
    return 0;
}
