#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
// #include <sys/wait.h>
#include <sys/wait.h>

// pritn the first 5 number by the child process and next 5 number by the parent process
int main(){
     
     int id=fork();
     int n ;

     if(id==0)
     {
        n=1;
     }
     else
     {    
         // wait(NULL);
         n=6;
  
     }

  if(id!=0)
     wait(NULL);
     
     for (int i = n; i <n+5; i++)
     {
        printf("%d\n",i);
        fflush(stdout);
     }

   
     return 0;
}