#include <iostream>
// #include <thread>
#include <pthread.h>
#include <atomic>
#include <bits/stdc++.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

atomic<bool> lock1(false);

pthread_mutex_t mutex1;
typedef struct
{
    // vector<int> ro vector<vector<int>> *rows;w;
    vector<int> *row;
    int *count1;
    int N;
    int K;
    int thread_no;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;
    vector<int> vector_row;
    vector<double> *time;
} arguments;

//   vector<bool> waiting[128];
  vector<bool> waiting(128, false);
  atomic<bool> key;

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
    
    //   pthread_mutex_unlock(&mutex1);
vector<int>::iterator it1;

auto start_time = std::chrono::high_resolution_clock::now();

    for (it1 = rowvector.begin(); it1 != rowvector.end(); it1++)
    {
        {
            for (int j = 0; j < N; j++)
            {
                long long sum = 0;

                for (int k = 0; k < N; k++)
                {
                    sum = sum + matrix[*it1][k] * matrix[k][j];
                }

                ans[*it1][j] = sum;
                sum = 0;
            }
        }
    }

     // Calculate the duration in microseconds

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
    int K = threadArgs->K;
    int thread_no = threadArgs->thread_no;
 

    while (*count1 <N-1)
    {
        
        waiting[thread_no] = true;
        key = true;
 
        while (waiting[thread_no] && key==true)
        {
     
            bool expected = false;
         
            key = !lock1.compare_exchange_strong(expected, true);
        }

        waiting[thread_no] = false;

        for (int i = 0; i < 16; i++)
        {
            // cout<<"chekc 3"<<endl;
            if(*count1>N-1)
            {
            break;
            }
            
           
            row->push_back(*count1);
            (*count1)++;
        }

        int j = (thread_no + 1) % K;
        while ((j != thread_no) && !waiting[j])
        {
            j = (j + 1) % K;
        }

        if (j == thread_no)
            lock1.store(false);
        else
            waiting[j] = false;

    }
// cout<<"chekc 6"<<endl;
    threadArgs->vector_row = *row;
    matrix_mult((void *)arg1);

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
    vector<double> time(K, 0);
    arguments arg1[K];

    // each thread will make this true when it will be waiting for the critical section
    //  bool waiting[K];
    // vector<bool> waiting(K, true);

    pthread_t thread[K];
 
    atomic<bool> key;
    key = true;

    for (int i = 0; i < K; i++)
    {
        arg1[i].count1 = &count;
        arg1[i].row = &rows[i];
        arg1[i].N = N;
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].K = K;
        arg1[i].time = &time;
        if (pthread_create(thread + i, NULL, &row_assign, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
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
