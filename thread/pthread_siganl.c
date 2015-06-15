#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* start1(void* arg)
{
    pthread_mutex_lock(&mutex);
    pthread_cond_wait(&cond,&mutex);
    printf("start1 has been called up\n");
    pthread_mutex_unlock(&mutex);
}

void* start2(void* arg)
{
    sleep(4);
    printf("thread2 sleep 4S,waiting for somthing\n");
    pthread_mutex_unlock(&mutex);
}

int main(int argc, const char *argv[])
{
    pthread_t tid1;
    pthread_create(&tid1, NULL, start1, NULL);
    pthread_t tid2;
    pthread_create(&tid2, NULL, start2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    printf("main thread over\n");
    return 0;
}
