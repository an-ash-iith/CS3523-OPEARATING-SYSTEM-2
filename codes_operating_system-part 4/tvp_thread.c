#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int x=2;

void* routine()
{
    x+=4;
    printf("value for x is %d\n",x);
    printf(" The message from thread pid is %d\n",getpid());
}

void* routine1()
{
    printf("value for x is %d\n",x);
    printf(" The message from thread pid is %d\n",getpid());
}

int main(){ 
    pthread_t t1,t2; 


    if(pthread_create(&t1,NULL,&routine,NULL)!=0)
    return 1;
   

     if(pthread_create(&t2,NULL,&routine1,NULL)!=0)
     return 2;


if(pthread_join(t1,NULL)!=0) 
return 3;

if(pthread_join(t2,NULL)!=0)
return 4;
     
     return 0;
}