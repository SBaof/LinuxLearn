#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<stdlib.h>
struct a
{
    int x;
    char *y;
};

void *creat(void *arg)
{
    struct a *temp;
    temp=(struct a *)arg;
    printf("x=%d\n",temp->x);
    printf("y=%s\n",temp->y);
    return (void *)0;
}

int main(int argc, const char *argv[])
{
    pthread_t pidt;
    int error;
    struct a *b;
    b=(struct a *)malloc(sizeof(struct a));
    b->x = 111;
    b->y = "fuck";
    error=pthread_create(&pidt, NULL, creat, (void *)b);
    if(error)
    {
        printf("error in create thread\n");
        exit(1);
    }
    //sleep(1);
    pthread_join(pidt,NULL);
    printf("pthread created successed\n");
    return 0;
}
