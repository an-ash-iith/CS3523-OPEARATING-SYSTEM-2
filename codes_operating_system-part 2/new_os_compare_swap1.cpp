#include <iostream>
#include <thread>
#include <pthread.h>
#include <atomic>
#include <bits/stdc++.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

pthread_mutex_t mutex1;

class SpinLock {
public:
    void lock() {
        bool expected = false;
        while (!lock_.compare_exchange_strong(expected, true, std::memory_order_acquire)) {
            // Busy wait until the lock becomes available
            expected = false;
        }
    }

    void unlock() {
        lock_.store(false, std::memory_order_release);
    }

private:
    // std::atomic<bool> lock_ = false;
     std::atomic<bool> lock_ = false;
};

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
    SpinLock* spin_lock;
} arguments;



// std::atomic<int> commonVariable(0);
std::atomic_flag flag = ATOMIC_FLAG_INIT;

void *matrix_mult(void *args)
{

    arguments *threadArgs = (arguments *)args;

    int N = threadArgs->N;
    int thread_no = threadArgs->thread_no;

    vector<vector<int>> &matrix = *(threadArgs->matrix);
    vector<vector<long long>> &ans = *(threadArgs->ans);
    vector<int> rowvector = threadArgs->vector_row;

    vector<int>::iterator it;

pthread_mutex_lock(&mutex1);
cout<<"thread no "<< thread_no<<" executing "<<endl;


    for (it = rowvector.begin(); it != rowvector.end(); it++)
    {
         cout<<" "<<*it<<" ";
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
    pthread_mutex_unlock(&mutex1);
    cout<<endl;

    return NULL;
}

void *row_assign(void *arg1)
{
    arguments* threadArgs = static_cast<arguments*>(arg1);
    // arguments *threadArgs = (arguments *)arg1;
    int *count1 = (threadArgs->count1);
    vector<int> *row = threadArgs->row;
    int N = threadArgs->N;
    int thread_no = threadArgs->thread_no;
    SpinLock* spin_lock = threadArgs->spin_lock;

    while (true)
    {   
        spin_lock->lock();
        // while (flag.test_and_set(std::memory_order_acquire))
        // {
        //     // wait
        // }
           
          if( *count1 > N - 1)
              {
                flag.clear(std::memory_order_release);
                break;

              }

        for (int i = 0; i < 16; i++)
        {
            row->push_back(*count1);
            (*count1)++;
        }

        // flag.clear(std::memory_order_release);
        // sleep(0.00001);
        spin_lock->unlock();
    }

threadArgs->vector_row=*row;
// matrix_mult()e
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

    pthread_t thread[K];
  pthread_mutex_init(&mutex1,NULL);
  SpinLock spin_lock;

    for (int i = 0; i < K; i++)
    {
        arg1[i].count1 = &count;
        arg1[i].row = &rows[i]; 
        arg1[i].N = N;
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].thread_no = i;
        arg1[i].spin_lock = &spin_lock;

        if (pthread_create(thread + i, NULL, &row_assign, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }
pthread_mutex_destroy(&mutex1);
// pthread_mutex_destroy(&mutex);
    // for (int i = 0; i < rows.size(); i++)
    // {
    //     cout<<"Thread_id is "<<i<<endl;
    //     for (int j = 0; j < rows[i].size(); j++)
    //     {
    //         cout << rows[i][j] << " ";
    //     }
    //     cout <<"K" <<endl;
    // }

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
