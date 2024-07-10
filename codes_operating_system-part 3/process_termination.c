#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
     
     pid_t id = fork();

     if(id==0)
     {
        printf("Welcome to the child process \n");
        printf("child process terminating  \n");

// void exit(int status);
        exit(4); //it is exiting with status 11
        // this exit status can be recoverd in parent proces usign wait() or waitpid() function
     }

     else
     {
        printf("Welcome to the parent  process \n");
 
 int status;

 waitpid(id,&status,0); // &sataus is pointer to variable where the status is stored
//  wait() system can also be called but it can be done if parent have only child process 

 printf("status returned is %d\n",status); 

//  Now, if you directly print the raw status value using printf, you might observe that the value is often a multiple of 256. This is because the exit status is represented in the lower 8 bits of the status value, and the higher bits contain other information.
// When you use printf("status returned is %d\n", status);, it prints the entire integer value of status, including both the exit status and the additional information. If you are observing multiples of 256, it likely means that the exit status is being multiplied by 256, and the result is stored in the higher bits.
// To print only the exit status, you should use the WEXITSTATUS macro:
// printf("Exit status is %d\n", WEXITSTATUS(status));
// This will extract the exit status portion of the status value and print it without any multiplication by 256.
  //for the given child id it will bring the status from the exit and thus known for successfull termination of the process
     
       if (WIFEXITED(status)) {  //check the staus that successfull termination has happended or not 
            int exit_status = WEXITSTATUS(status);
            printf("Child process (pid %d) exited with status: %d\n", id, exit_status);
        }


     }
     return 0;
}