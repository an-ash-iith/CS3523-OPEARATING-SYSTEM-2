#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>


int value =5;

int main(){
     pid_t pd;
     pd=fork();
     
     if(pd==0)
     {
        value+=15;
     }
     else
     {
        wait(NULL);
        printf("Value = %d\n",value);
     }

     return 0;
}