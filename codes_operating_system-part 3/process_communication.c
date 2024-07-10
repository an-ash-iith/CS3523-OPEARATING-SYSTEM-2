#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>

// here we will be doing communication usign pipe(). first we will deal with ordinary pipe which is unidirectional
// one end of pipe used for reading while other for writing
int main(){

    int fd[2]; //file descrypter of write end and other for file descrypter of read end

// fd[0]  --read
// fd[1]  --write

// Create a one-way communication channel (pipe).
// If successful, two file descriptors are stored in PIPEDES;
// bytes written on PIPEDES[1] can be read from PIPEDES[0].
// Returns 0 if successful, -1 if not.

if (pipe(fd)==-1)
{
  printf("Error in opening the pipe \n");return 1;

}

int id=fork();

if(id==0)
{
    close(fd[0]);
    int x;
    printf("Input a number: ");
    scanf("%d",&x);
    write(fd[1],&x,sizeof(int));
    close(fd[1]);

}
else{

    close(fd[1]);
    int y;
    read(fd[0],&y,sizeof(int));
    close(fd[0]);
    printf("Got from child procee  %d\n",y);
}

     return 0;
}