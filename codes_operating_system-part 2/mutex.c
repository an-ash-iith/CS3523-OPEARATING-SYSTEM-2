#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int mail=0;
int mail2=0;

// observation when you will not unlock the mutex then first thread will lock the things and will not releasae the lock 
// but will complte the execution means the work given to thread will be done thus pthread_join will properly delete the thread
//but second thread will be stiil waiting for the first thread to release the lock and thus will not complte its work ever thus 
//the function it call will not return and thus if you call joun_thread for that then it wiill get stuck as join thread store 
//return value from the thread. thus whole program will stuck .
//if you remove that join_thread it will exevute but will not give correct code


pthread_mutex_t mutex;

// race condition will occur only on multicore system 
// try to avoid mutex if possible because it will slow the code 

void* routine()
{

    //why its not working for 10000 for working form 1000 ..the point is when you are creating other thread till then first 
    //thread is completing its work so no differnence on the data is coming ------

pthread_mutex_lock(&mutex);


    for (int i = 0; i < 100000; i++)
    {
        mail++;
    }

    pthread_mutex_unlock(&mutex);  //if you not call unlock function then it will be lock thus and stuck as other thread cant access it anymore 
    // then you have to generate the keyboard interrupt to terminate

    
}

    int main(){ 
    pthread_t t1,t2; 

pthread_mutex_init(&mutex,NULL); //calling inbuilt mutex

     if(pthread_create(&t1,NULL,&routine,NULL)!=0)
     return 1;
   

     if(pthread_create(&t2,NULL,&routine,NULL)!=0)
     return 2;

printf("helo1 \n");

if(pthread_join(t1,NULL)!=0) 
return 3;

printf("helo2 \n");

if(pthread_join(t2,NULL)!=0)
return 4;

printf("mail = %d , mail2= %d\n",mail,mail2);

pthread_mutex_destroy(&mutex);
     
pthread_mutex_init(&mutex,NULL); //destroying the mutex

     printf("mail = %d , mail2= %d\n",mail,mail2);
     return 0;
}