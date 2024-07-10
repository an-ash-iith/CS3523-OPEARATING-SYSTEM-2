#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>

#define number_of_core as nc
#define nc 8

using namespace std;

pthread_mutex_t mutex;

typedef struct
{
    int row1;
    int row2;
    int N;
    vector<vector<int>> *matrix;
    vector<vector<long long>> *ans;
    int *thread_id;
    vector<double> *time;

} arguments;

// section shows matrix row to be calculated after multiplication ---

void *matrix_mult(void *args)
{

    arguments *threadArgs = (arguments *)args;
    int N = threadArgs->N;
    int start_row = threadArgs->row1;
    int end_row = threadArgs->row2;
    int thread_no = *(threadArgs->thread_id);
    vector<vector<int>> &matrix = *(threadArgs->matrix);
    vector<vector<long long>> &ans = *(threadArgs->ans);
    vector<double> &time = *(threadArgs->time);

    // starting the time
    auto start_time = std::chrono::high_resolution_clock::now();

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

    // Calculate the duration in microseconds

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_time = end_time - start_time;

    time[thread_no] = elapsed_time.count();

    return NULL;
}

vector<vector<int>> readmatrix(int &N, int &K, int &C, int &BT, string &file1)
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
    input >> N >> K >> C >> BT;

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
    int C;
    int BT;

    string filename = "inp.txt";

    // passing N and F so that can be assigned
    vector<vector<int>> matrix = readmatrix(N, K, C, BT, filename);
    vector<vector<long long>> ans(N, vector<long long>(N));

    // making the time vector to note down the time of size K as for each thread and initilizing with 0 for avoiding any ambiguity
    vector<double> time(K, 0);

    cout << N << " " << K << " " << C << " " << BT << endl;


    pthread_t thread[K];

    arguments arg1[K];

    int dist = ceil((float)N / K);


    // number of the threads get by each processor

    cout << "thread creating" << endl;

    // distributing the rest of the thread K- BT to the other cores without bounding
    for (int i = 0; i < K; i++)
    {
        arg1[i].matrix = &matrix;
        arg1[i].ans = &ans;
        arg1[i].N = N;
        arg1[i].row1 = dist * i;
        arg1[i].row2 = dist * (i + 1);
        arg1[i].thread_id = new int(i);
        arg1[i].time = &time;

        if (pthread_create(thread + i, NULL, &matrix_mult, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }



    cout << "thread executed" << endl;

    // finding the average of the time taken by each thread stored in vector
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