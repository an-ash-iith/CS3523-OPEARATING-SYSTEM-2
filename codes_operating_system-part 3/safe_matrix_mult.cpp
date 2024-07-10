#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>

using namespace std;

typedef struct
{
    int row1;
    int row2;
    int N;
    vector<vector<int>> *matrix;
    vector<vector<int>> *ans;

    //  vector<vector<int>> &ans;  you cant do this becauese this are instances passing address 
} arguments;

// int ans[N][N];

void *matrix_mult (void * args)
{
// void *matrix_mult(arguments (void * args))

     arguments *threadArgs = (arguments *)args;

    int N = threadArgs->N;
    int start_row= threadArgs->row1;
    int end_row= threadArgs->row2;
    cout<<N<<" "<<start_row<<" "<<end_row<<endl;

    vector<vector<int>> &matrix=*(threadArgs->matrix);
    vector<vector<int>> &ans=*(threadArgs->ans);

   
    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0;

            for (int k = 0; k< N; k++)
            {
                sum = sum + matrix[i][k] * matrix[k][j];
            }

            ans[i][j] = sum;
            sum = 0;
        }
    }

cout<<"returning from thread "<< start_row<<endl;

    return NULL;

}


int main()
{
    int N;
    int K;

    cout << "Enter the Rows" << endl;
    cin >> N;

     cout << "Enter the Threads" << endl;
     cin >> K;

    vector<vector<int>> matrix(N, vector<int>(N));

    vector<vector<int>> ans(N, vector<int>(N));

    cout << "Enter the Matrix" << endl;

    // our matrix is the square matrix ------

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> matrix[i][j];
        }
    }

// number of thread 


pthread_t thread[K];

arguments arg1[K];

int dist= ceil((float)N/K);
cout<<dist<<endl;

cout<<"thread creating"<<endl;
for (int  i = 0; i < K; i++)
{
    arg1[i].matrix= &matrix;
    arg1[i].ans= &ans;
    arg1[i].N=N;
    arg1[i].row1=dist*i;
    arg1[i].row2=dist*(i+1);
    
    if(i==K-1)
    arg1[i].row2=N;
    
    if(pthread_create(thread+i,NULL,&matrix_mult,(void *)&arg1[i])!=0)
    return 7;
}


for (int  i = 0; i < K; i++)
{
    
    if(pthread_join(thread[i],NULL)!=0)
    return 8;
}
cout<<"thread terminated"<<endl;

cout<<"This is your multiplied matrix"<<endl;

     for (int  i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
           cout<<ans[i][j]<<" ";
        }
        cout<<endl;
    }

    return 0;
}