// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"
// #include "kernel/fcntl.h"
// #include <kernel/file.h>
// Function to create a file with the given name and size

//function for the matrix multiplication --
void file_create2(char *filename, int size) {
    int fd = open(filename, O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf("Error creating file\n");
        exit(0);
    }

    char buffer[size_of_block];
   

    // Write the content size to all disk blocks
    for (unsigned int i = 0; i < size_of_block; i++) {

        //storing to the buffer as each charactr have size of 1 byte 
        //each block contain one 512 character if present
    }

    // Write the buffer to the file
    for (unsigned int i = 0; i < size; i += size_of_block) {
        write(fd, buffer, size_of_block);
    }

    close(fd);
}

void matrix_mult(char *file_name1,char *file_name2,char *file_name3)
{     
    int fd1 = open(filename1, O_CREATE | O_WRONLY);
    int fd2 = open(filename2, O_CREATE | O_WRONLY);
    int fd3 = open(filename3, O_CREATE | O_WRONLY);

        for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            long long sum = 0;

            for (int k = 0; k < N; k++)
            {
                sum = sum + matrix[i][k] * matrix[k][j];
            }

            ans[i][j] = sum;
            sum = 0;
        }
    }

}

void file_create(char *filename, int size ,pagetable_p *p ,int file_no) {
    
    //creating the file
    int fd = open(filename, O_CREATE | O_WRONLY);
    if (fd < 0) {
        printf("Error creating file\n");
        exit(0);
    }
 
 //getting the page tabel for the currently allocated file 

       p[file_no] = myproc();

    close(fd);
}


int main(){
     
  int dimA_row, dimA_col;
  int dimB_row,dimB_col;

//taling input from the user 

 //lets make matrix of size A 

 

 //creting three file name --
//  uvmalloc(p->pagetable, sz, sz + n, PTE_W)) == 
    pagetable_t p[3];

  //initially not allocating any memory 

    char *file_name = "matA.txt";
    file_create(file_name, 0,p[0],0);

    char *file_name = "matB.txt";
    file_create(file_name, 0,p[1],1);

    char *file_name = "matC.txt";
    file_create(file_name, 0,p[2],2);

   printf("Enter the size of matrix A and B\n");


prinntf("Enter the dimension of Matrix A :");
scanf("%d %d",&dimA_row ,&dimeA_col );


printf("Enter the dimension of Matrix B :");
scanf("%d %d",&dimB_row ,dimB_col );

   //as have dimA as dimensiion 
   //number of element will be dimA*dimA and assuming each entry has type int
   //so multiplying by 4
uvmalloc(p[0],0,dimA_row*dimA_col*4,PTE_W);

uvmalloc(p[1],0,dimB_row*dimB_col*4,PTE_W);


//after multiplying the row of c will become row of A and column if c will 
//become column of B

uvmalloc(p[2],0,dimA_row*dimB_col*4,PTE_W);

//writing the matrix into the file A and B 
 

    
     return 0;
}
