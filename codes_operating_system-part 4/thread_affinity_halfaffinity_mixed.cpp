#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <time.h>
#include <fstream>
#define number_of_core as nc
#define nc 8

using namespace std;

typedef struct
{
    int thread_no;
    int N;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;
    vector<vector<int>> *rows;
    vector<int> vector_row;
    int *thread_id;

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
    auto start_time1 = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < K / 2; i++)
    {
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].vector_row = rows[i];
        arg1[i].thread_id = new int(i);

        // arg1[i].thread_id = &i;   if you do this then it is not guarantee that you will
        // get right value becasue this is not the dedicated address and something can be written
        // on thsi address by other function -----

        if (pthread_create(thread + i, NULL, &matrix_mult, (void *)&arg1[i]) != 0)
            return 2;
        int *threadId = new int(i);

        // Set the CPU affinity for this thread
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);

        CPU_SET((*arg1[i].thread_id) % nc, &cpuset);

        // 0.463 second time it is taking
        pthread_setaffinity_np(*(thread + i), sizeof(cpu_set_t), &cpuset);
        //         // usleep(100);
    }

    for (int i = 0; i < K/2; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
    
     auto end_time1 = std::chrono::high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>(end_time1 - start_time1);


    auto start_time2 = std::chrono::high_resolution_clock::now();

    for (int i = K / 2; i < K; i++)
    {
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].vector_row = rows[i];
        arg1[i].thread_id = new int(i);

        // arg1[i].thread_id = &i;   if you do this then it is not guarantee that you will
        // get right value becasue this is not the dedicated address and something can be written
        // on thsi address by other function -----

        if (pthread_create(thread + i, NULL, &matrix_mult, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = K/2; i < K; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }

    // clock_t final = clock();
    auto end_time2 = std::chrono::high_resolution_clock::now();

    // Calculate the duration in microseconds
    auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time2);
    auto duration3 = std::chrono::duration_cast<std::chrono::microseconds>(end_time2 - start_time1);

    // Print the duration in microseconds
    std::cout << "Time taken by the program with affinity: " << duration1.count() << " microseconds" << std::endl;
    std::cout << "Time taken by the program without affinity: " << duration2.count() << " microseconds" << std::endl;
      std::cout << "Time taken by the program total: " << duration3.count() << " microseconds" << std::endl;
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