#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <fstream>

using namespace std;

typedef struct
{
    int thread_no;
    int N;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;
    vector<vector<int>> *rows;
    vector<int> vector_row;

} arguments;

void *matrix_mult(void *args)
{

    arguments *threadArgs = (arguments *)args;

    int N = threadArgs->N;
    int thread_no = threadArgs->thread_no;

    vector<vector<int>> &matrix = *(threadArgs->matrix);
    vector<vector<long long>> &ans = *(threadArgs->ans);
    vector<int> rowvector = threadArgs->vector_row;

    vector<int>::iterator it;

    for (it = rowvector.begin(); it != rowvector.end(); it++)
    {

        {
            for (int j = 0; j < N; j++)
            {
                long long sum = 0;

                for (int k = 0; k < N; k++)
                {
                    sum = sum + matrix[*it][k] * matrix[k][j];
                }

                ans[*it][j] = sum;
                sum = 0;
            }
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

    // here N represents size of matrix row and K represnt number of thread
    int N;
    int K;
    string filename = "inp.txt";

    // passing N and F so that can be assigned 
    vector<vector<int>> matrix = readmatrix(N, K, filename);
    vector<vector<long long>> ans(N, vector<long long>(N));

    // our matrix is the square matrix ------

  
    int dist = ceil((float)N / K);
    vector<vector<int>> rows(K);

    pthread_t thread[K];
    arguments arg1[K];

    // distributing each rows to their resepective thread

    for (int i = 0; i < N; i++)
    {
        rows[i % K].push_back(i);
    }

    // number of thread

    cout << "thread creating" << endl;

    // clock_t initial = clock();
    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < K; i++)
    {
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].vector_row = rows[i];

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
    cout << "thread terminated" << endl;

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