#include <iostream>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <bits/stdc++.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

atomic<bool> lock1(false);

pthread_mutex_t mutex1;
typedef struct
{
    // vector<int> ro vector<vector<int>> *rows;w;
    vector<int> *row;
    // vector<vector<int>> *rows;
    int *count1;
    int N;
    int thread_no;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;
    vector<int> vector_row;
      vector<double> *time;
} arguments;



// std::atomic<int> commonVariable(0);
// std::atomic_flag flag = ATOMIC_FLAG_INIT;

void *matrix_mult(void *args)
{

    arguments *threadArgs = (arguments *)args;

    int N = threadArgs->N;
    int thread_no = threadArgs->thread_no;

    vector<vector<int>> &matrix = *(threadArgs->matrix);
    vector<vector<long long>> &ans = *(threadArgs->ans);
    vector<int> rowvector = threadArgs->vector_row;
    vector<double> &time = *(threadArgs->time);
  auto start_time = std::chrono::high_resolution_clock::now();
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
    
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_time = end_time - start_time;

    time[thread_no] = elapsed_time.count();
    return NULL;
}


void *row_assign(void *arg1)
{
    arguments *threadArgs = (arguments *)arg1;
    int *count1 = (threadArgs->count1);
    vector<int> *row = threadArgs->row;
    int N = threadArgs->N;
    int thread_no = threadArgs->thread_no;

    while (true)
    {
  
    bool expected = false;
    bool desired = true;
    while (!lock1.compare_exchange_strong(expected, desired)) {
        expected = false;
        desired = true;
    }
           
          if( *count1 > N - 1)
              {
                lock1.store(false);
                break;

              }

        for (int i = 0; i < 16; i++)
        {
            row->push_back(*count1);
            (*count1)++;
        }

        lock1.store(false);
        
    }

threadArgs->vector_row=*row;
matrix_mult((void *)arg1);

    return NULL ;
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

// pthread_mutex_t mutex;
int main()
{

        // here N represents size of matrix row and K represnt number of thread
    int N;
    int K;
    string filename = "inp.txt";
    
    int count = 0;

    // pthread_mutex_init(&mutex, NULL);
    // passing N and F so that can be assigned 
    vector<vector<int>> matrix = readmatrix(N, K, filename);
    vector<vector<long long>> ans(N, vector<long long>(N));
    vector<vector<int>> rows(K);
    arguments arg1[K];
   vector<double> time(K, 0);
    pthread_t thread[K];
  pthread_mutex_init(&mutex1,NULL);

    for (int i = 0; i < K; i++)
    {
        arg1[i].count1 = &count;
        arg1[i].row = &rows[i]; 
        arg1[i].N = N;
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].time = &time;

        if (pthread_create(thread + i, NULL, &row_assign, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
pthread_mutex_destroy(&mutex1);

    double sum = 0;

    for (int l = 0; l < K; l++)
    {
        sum = sum + time[l];
        cout << time[l] << endl;
    }

    // taking average of the time --by dividing by K(number of threads)
    std::cout << "Time taken by this: " << sum * 1000 / K << " milliseconds" << std::endl;


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
