#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>
#include <cmath>
#include <fstream>
#include <time.h>

using namespace std;

FILE *fptr1 = fopen("outfile.txt", "w");
// fptr1 =

typedef struct
{
    int limit1;
    int limit2;
    int N;

    vector<vector<int>> *ans;
    int thread_no;
    vector<double> *time;
} arguments;

// section shows matrix row to be calculated after multiplication ---

bool checkPerfect(int val)
{
    int sum = 0;

    for (int i = 1; i < val; i++)
    {
        if (val % i == 0)
        {
            sum += i;
        }
    }

    if (sum == val)
        return true;

    else
        return false;
}

void *findPerfect(void *args)
{

    arguments *threadArgs = (arguments *)args;
    int limit1 = threadArgs->limit1;
    int limit2 = threadArgs->limit2;
    int thread_no = threadArgs->thread_no;

    vector<vector<int>> &ans = *(threadArgs->ans);

    vector<double> &time = *(threadArgs->time);

    auto start_time = std::chrono::high_resolution_clock::now();

    for (int i = limit1; i < limit2; i++)
    {
        if (checkPerfect(i) == true)
            fprintf(fptr1, "%d Found by Thread %d \n", i, thread_no); // directly printing into file
    }

    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> elapsed_time = end_time - start_time;
   
  
    time[1] = elapsed_time.count();

    return NULL;
}

void readinput(int &N, int &K, string &file1)
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

    return;
}

int main()
{

    int N;
    int K;

    string filename = "inp.txt";

    // passing N and F so that can be assigned
    readinput(N, K, filename);

    vector<vector<int>> ans(N, vector<int>(0));
    vector<double> time(K, 0);

    pthread_t thread[K];

    arguments arg1[K];
    
    int dist = ceil((float)N / K);

    FILE *fptr;
    fptr = fopen("outfile.txt", "w");

    cout << "thread creating" << endl;

    for (int i = 0; i < K; i++)
    {

        arg1[i].N = N;
        arg1[i].limit1 = dist * i + 1;
        arg1[i].limit2 = dist * (i + 1) + 1;
        arg1[i].thread_no = i;
         arg1[i].time = &time;

        if (i == K - 1)
            arg1[i].limit2 = N + 1;

        if (pthread_create(thread + i, NULL, &findPerfect, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {

        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }

    fclose(fptr1);
    cout << "thread executed" << endl;

    double sum = 0;

    for (int l = 0; l < K; l++)
    {
        sum = sum + time[l];
    }

    // taking average of the time --by dividing by K(number of threads)
    std::cout << "Time taken by this: " << sum * 1000 / K << " milliseconds" << std::endl;

    return 0;
}