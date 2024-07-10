#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>


int mail=10000;
pthread_mutex_t mutex;
void *routine()
{
 printf("welcome to routine betaji \n");
 
 pthread_mutex_lock(&mutex);
 for (int  i = 0; i < 10000; i++)
 {
    mail++;
 }
  pthread_mutex_unlock(&mutex);
 sleep(1);
 printf("ending\n");
}


int main(){ 
pthread_t t1,t2; 
pthread_t th[4];

int i ;
pthread_mutex_init(&mutex,NULL); //calling inbuilt mutex

//creating 4 threads----->

for (int i = 0; i < 4; i++)
{

     if(pthread_create(th+i,NULL,&routine,NULL)!=0)
     return 1;
}

// printf(" Thread has been created \n");


for (int i = 0; i < 4; i++)
{
     if(pthread_join(th[i],NULL)!=0)
     return 2;

     printf("Thread %d has finished execution \n",i);
}

pthread_mutex_destroy(&mutex); //calling inbuilt mutex
// note that it is not necessary that thread will end in order 
// but due to for loop its showing in order 

printf(" All thread has been terminated \n");
printf("Mails are  %d \n",mail);

     return 0;
}