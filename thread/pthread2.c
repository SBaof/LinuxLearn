#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* pthread_start(void* arg)
{
    printf("thread id is : %lu\n", pthread_self());
    printf("thread will sleep for 2S\n");
    sleep(2);
    pthread_exit((void* )100);
}

int main(int argc, const char *argv[])
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, pthread_start, NULL)!=0)
    {
        perror("error in create thread\n");
        exit(1);
    }
    void* val=NULL;
    if(pthread_join(tid, &val)!=0)
    {
        perror("error in pthread_join \n");
        exit(1);
    }
    printf("this is in main thread\n");
    printf("the exit num of thread is : %d\n", (int)val);
    return 0;
}
