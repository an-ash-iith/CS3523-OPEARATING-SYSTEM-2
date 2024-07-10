#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace std;

typedef struct
{
    int row1;
    int row2;
    int N;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;

} arguments;

// section shows matrix row to be calculated after multiplication ---

void *matrix_mult(void *args)
{

    arguments *threadArgs = (arguments *)args;
    int N = threadArgs->N;
    int start_row = threadArgs->row1;
    int end_row = threadArgs->row2;

    vector<vector<int>> &matrix = *(threadArgs->matrix);
    vector<vector<long long>> &ans = *(threadArgs->ans);

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

    return NULL;
}

vector<vector<int>> readmatrix(int &N, int &K, string &file1)
{

    // making the instance of the ifstream named input and taking input as file1
    ifstream input(file1);

    // if file is already open by other application then will through the error

    if (!input.is_open())
    {
        cerr << "file opening failed ..." << endl;
        exit(EXIT_FAILURE);
    }

    // Read the number of rows and columns from the first line of the file
    input >> N >> K;

    // makig 2d vector named matrix of size N * N
    vector<vector<int>> matrix(N, vector<int>(N));

    // Loop through the file and read each element into the matrix
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            input >> matrix[i][j];
        }
    }

    // Closing the file
    input.close();

    return matrix;
}

int main()
{

    int N;
    int K;

    string filename = "inp.txt";

    // passing N and F so that can be assigned 
    vector<vector<int>> matrix = readmatrix(N, K, filename);

    vector<vector<long long>> ans(N, vector<long long>(N));

    // started the clock for measuring the time
    // clock_t initial = clock();
    auto start_time = std::chrono::high_resolution_clock::now();

    pthread_t thread[K];

    arguments arg1[K];

    int dist = ceil((float)N / K);

    cout << "thread creating" << endl;
    for (int i = 0; i < K; i++)
    {
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].row1 = dist * i;
        arg1[i].row2 = dist * (i + 1);
        
        if (i == K - 1)
            arg1[i].row2 = N;

        if (pthread_create(thread + i, NULL, &matrix_mult, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
  // clock_t final = clock();
   auto end_time = std::chrono::high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);

    // Print the duration in microseconds
    std::cout << "Time taken by the program: " << duration.count() << " microseconds" << std::endl;
  
    cout << "thread executed" << endl;

    FILE *fptr;
    fptr = fopen("outfile.txt", "w");

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
          
            fprintf(fptr, "%lld ", ans[i][j]);
        }
       
        fprintf(fptr, "\n");
    }

    return 0;
}