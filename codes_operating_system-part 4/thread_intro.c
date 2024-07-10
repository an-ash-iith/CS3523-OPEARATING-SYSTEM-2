#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void* routine()
{
    printf("This is the message from thread\n");
    sleep(2);
    printf("This is the second message\n");
}

int main(){ 
    pthread_t t1,t2; //this is struct variable that store information about thread


// if thread is successfully created then it return 0 else it return 1
    if(pthread_create(&t1,NULL,&routine,NULL)!=0)
    return 1;
     //first parameter is reference to struct variable pthread_t  and second is pointer 
    //that give information about prioroty , third is passing the function or process to be exexute on that thread // 4th one is 
    // 4th argument is the parameter to the proceess or function here 

     if(pthread_create(&t2,NULL,&routine,NULL)!=0)
     return 2;
// it is good practice to check the creation of the thread


if(pthread_join(t1,NULL)!=0) //return value from the process as here void function is there so null pointer 
return 3;

if(pthread_join(t2,NULL)!=0)
return 3;
     
     return 0;
}