#include<stdio.h>

int main(int argc, const char *argv[])
{
    //int *p=(int*)0x12ff7c;
    //*p=NULL;
    //p=NULL;
    //int i;
    int *p=(int *)0xbffff110;
    *p=1234;
    p=NULL;

    return 0;
}
