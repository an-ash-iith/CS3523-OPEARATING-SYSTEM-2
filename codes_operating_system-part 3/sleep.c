#include "kernel/types.h"
#include <kernel/stat.h>
#include <user/user.h>

int main() {
    // for each direction one pipe 
    int pipe1[2], pipe2[2];

    // Create pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        fprintf(2, "Error: Failed to create pipes\n");
        exit(1);
    }

    // child process
    int pid = fork();

    if (pid == 0) {
        close(pipe1[1]); // write end for the child process
        close(pipe2[0]); // read end for the child process

        char read_child[1]; // character to read 
        read(pipe1[0], read_child, 1);
        printf("%d: recieved ping\n" , getpid());
        write(pipe2[1], "pong", 1);
     
        close(pipe1[0]);
        close(pipe2[1]);
        exit(0);
    } else {
        close(pipe1[0]); // read end for the parent process
        close(pipe2[1]); // write end for the parent process

        char read_parent[1]; // character to read 
        write(pipe1[1], "ping", 1);
        read(pipe2[0], read_parent, 1);
        printf("%d: recieved pong\n" , getpid());

        wait(0);
        close(pipe1[1]);
        close(pipe2[0]);
        exit(0);
    }

    return 0;
}
