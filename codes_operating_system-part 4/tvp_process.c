#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <wait.h>

int main(){
 int x=2;
     int pid=fork();

     if(pid==-1)
     return -1;

     printf("Hello from process pid is %d\n",getpid());


if(pid==0)
x++;


printf("value for x is %d\n",x);
     if(pid!=0)
     wait(NULL);
     
     return 0;
}