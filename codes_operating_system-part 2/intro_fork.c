#include <stdio.h>
#include <unistd.h>
int main(int argc, char* argv[]){

    int id = fork();  //this will make the child and in parent process return the process id of
    //child process while in child it will return 0 


// getpid()  function return the current process id that is reading this function 
//getppid() function return parent id or the process that have created this process
    if(id==0)
    {
        printf("welcome to the child process, id return = %d , process id is = %d  ,parent id is =%d \n",id,getpid(),getppid());
   
    } 
    else
    {
         printf("welcome to the parent process , id return = %d , process id is = %d ,parent id is =%d  \n",id,getpid(),getppid());
    }
  

    
    if(id!=0)
    {
        sleep(1);
        printf("awake\n");
    }
     
     return 0;
}