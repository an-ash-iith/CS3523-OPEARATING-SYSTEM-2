#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

// aim is to create three peocess and find the sum of the given array using dividing 
int main(){
     
int arr[]={9,1,3,6,2,12,11,5,8};
int n = sizeof(arr)/sizeof(int);
int count;
int start;
int id2=-1;

int fd1[2];
int fd2[2];
pipe(fd1);
pipe(fd2);
int id1 = fork();

if(id1!=0)  //parent process
{
start=0;
count=n/3;
 id2=fork();
}

if(id1==0)  //first child process
{
    start=3;
    count=n/3;
}

if(id2==0)  //second child process
{
    start=6;
    count=n/3;
}

 int sum=0;

for (int i = start; i <start+count; i++)
{ 
sum=sum+arr[i];
}

printf("The sum of this section is %d %d %d\n",sum,id1,id2);

// The sum of this section is 8 5902 5903  --this is the parent one
// The sum of this section is 26 0 -1  --first child of the parent
// The sum of this section is 24 5902 0  --second child of the parent you can see id1 =getpid() of first child but this is not a parent just copy value of parent came into this as it it created at last



if(id1==0)
{ 
    close(fd1[0]);
    write(fd1[1],&sum,sizeof(int));
    close(fd1[1]);
    close(fd2[1]);
    close(fd2[0]);
}

if(id2==0)
{ 
    close(fd2[0]);
    write(fd2[1],&sum,sizeof(int));
    close(fd2[1]);
    close(fd1[0]);
    close(fd1[1]);
}

if(id2!=0 && id1!=0)
{ 
    int sum1;
    int sum2;

    close(fd1[1]);
    close(fd2[1]);

    wait(NULL);
    read(fd1[0],&sum1,sizeof(int));
    read(fd2[0],&sum2,sizeof(int));

    close(fd1[0]);
    close(fd2[0]);

    printf("The total sum is %d \n",sum+sum1+sum2);
}

     return 0;
}