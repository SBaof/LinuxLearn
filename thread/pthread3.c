#include<stdio.h>
#include<pthread.h>
#include<stdio.h>

void* pthread_start(void* arg)
{
    printf("this is in pthread,my id is: %lu\n",pthread_self());
    printf("again and again\n");
    int n;
    for(n=1; ; n++)
    {
        printf("n=%d\n",n);
        sleep(1);
    }
}

int main(int argc, const char *argv[])
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, pthread_start, NULL)!=0)
    {
        perror("error in create pthread\n");
        return 1;
    }
    printf("this is in main thread\n");
    printf("sleep 5S\n");
    sleep(5);
    if(pthread_cancel(tid)!=0)
    {
        perror("pthread cancled error\n");
        return 1;
    }
    return 0;
}
