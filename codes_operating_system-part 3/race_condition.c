#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int mail=0;
int mail2=0;

void* routine()
{

    //why its not working for 10000 for working form 1000 ..the point is when you are creating other thread till then first 
    //thread is completing its work so no differnence on the data is coming ------

    for (int i = 0; i < 1000; i++)
    {
        mail++;
    }

     for (int i = 0; i < 10000; i++)
    {
        mail2++;
    }
    
}



int main(){ 
    pthread_t t1,t2; 


    if(pthread_create(&t1,NULL,&routine,NULL)!=0)
    return 1;
   

     if(pthread_create(&t2,NULL,&routine,NULL)!=0)
     return 2;


if(pthread_join(t1,NULL)!=0) 
return 3;

if(pthread_join(t2,NULL)!=0)
return 4;
     

     printf("mail = %d , mail2= %d\n",mail,mail2);
     return 0;
}