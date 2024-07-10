#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
     
     printf("welcome to the second exec\n");
     printf("the pid second procees is= %d\n",getpid());
     return 0;
}