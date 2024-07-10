#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
    //  int id = fork();
     printf("Tis is the first program\n");
     printf("First program has pid = %d \n",getpid());
     char *args[]={"hello","hi"};

     execv("./exec_lec2",args);  //first argument is compiled or executable file of the other program so be carefull with the name 
     printf("I will not get printed\n");

    //  you will find above statement has not been executed and pid of the both is same i
    // what actully happens is when exec() is called after that the content of the first program get removed and content of other program whose executable in parameter come into play
     return 0;
}