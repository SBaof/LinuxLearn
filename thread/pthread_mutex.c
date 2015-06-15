#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#include<time.h>

typedef struct data
{
    int n;
    pthread_mutex_t lock;
}DATA;

DATA* init_lock(void)
{
    DATA* p=NULL;
    p=(DATA* )malloc(sizeof(DATA));
    p->n = 200;
    if(pthread_mutex_init(&(p->lock), NULL)!=0)
    {
        perror("pthread_mutex_init error\n");
        return NULL;
    }
    return p;
}

void* pthread_start1(void* arg)
{
    DATA* p = arg;
    pthread_mutex_lock(&(p->lock));
    printf("this is in thread1, n=%d\t",p->n);
    printf("start1 thread sleep 5S,then change n from 200 to 1000\n");
    sleep(5);
    p->n = 1000;
    pthread_mutex_unlock(&(p->lock));
}

void* pthread_start2(void* arg)
{
   // sleep(2);
    DATA* p = arg;
    pthread_mutex_lock(&(p->lock));
    printf("start2 read num is: %d\n", p->n);
    pthread_mutex_unlock(&(p->lock));
}

int main(int argc, const char *argv[])
{
    DATA* p = NULL;
    if((p=init_lock())==NULL)
    {
        perror("init_lock error\n");
        return 1;
    }
    pthread_t tid1;
    if(pthread_create(&tid1, NULL, pthread_start1, p)!=0)
    {
        perror("error in create thread1\n");
        return 1;
    }
    pthread_t tid2;
    if(pthread_create(&tid2, NULL, pthread_start2, p)!=0)
    {
        perror("error in create thread2\n");
        return 1;
    }
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("in main, n=%d\n",p->n);
    pthread_mutex_destroy(&(p->lock));
    free(p);
    printf("main thread over\n");
    return 0;
}
