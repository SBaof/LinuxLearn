#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
#define MAX 512

typedef struct data
{
    int i;
    float f;
    char str[MAX];
}DATA;

void *pthread_start(void* arg)
{
    DATA* p=arg;
    printf("in thread, i=%d, f=%f, str=%s\n", p->i, p->f, p->str);
    printf("this is in thread, my id is: %lu\n",pthread_self());
    return NULL;
}

int main(int argc, const char *argv[])
{
    pthread_t tid;
    DATA d={10, 15.5f, "nimei"};
    if((pthread_create(&tid, NULL, pthread_start, &d))!=0)
    {
        perror("error in create pthread\n");
        exit(1);
    }
    sleep(1);
    printf("this is in main thread\n");
    int n;
    for(n=1; ; n++)
    {
        printf("n=%d\n",n);
        sleep(1);
    }

    return 0;
}
