#include <stdio.h>
#include <stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include <pthread.h>

int main()
{

//for each direction one pipe 
int pipe1[2],pipe2[2];

//child process
int pid = fork();

//this is a child process 
if(fork()==0)
{
    close(pipe1[1]); //write end for the child process
    close(pipe2[0]); //read end for the child process
   
   char read_child[1]; //character to read 
    read(pipe1[0],read_child,1);
    write(pipe2[1],"pong",1);

    close(pipe1[0]);
    close(pipe2[1]);
    exit(0);
}

else
{
    close(pipe1[0]);
    close(pipe2[1]);
   
   char read_parent[1]; //character to read 
    write(pipe1[1], "ping" ,1); 
    read(pipe2[0],read_parent,1);

    wait(0);
    close(pipe1[1]);
    close(pipe2[0]);
    exit(0);


}


return 0;
}