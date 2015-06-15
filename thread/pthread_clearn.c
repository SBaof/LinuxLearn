#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void* clean1(void* arg)
{
    printf("clean1\n");
    return NULL;
}

void* clean2(void* arg)
{
    printf("clean2\n");
    return NULL;
}

void* pthread_start(void* arg)
{
    pthread_cleanup_push(clean1, NULL);
    pthread_cleanup_push(clean2, NULL);
    pthread_exit(NULL);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t tid;
    if(pthread_create(&tid, NULL, pthread_start, NULL)!=0)
    {
        perror("error in create pthread\n");
        return 1;
    }
    pthread_join(tid, NULL);
    return 0;
}
