#include <iostream>
#include <thread>
#include <atomic>
#include <bits/stdc++.h>
#include <time.h>
using namespace std;

typedef struct
{
    // vector<int> ro vector<vector<int>> *rows;w;
    vector<int> *row;
    int *count1;
    int N;
} arguments;

// std::atomic<int> commonVariable(0);
std::atomic_flag flag = ATOMIC_FLAG_INIT;

void *row_assign(void *arg1)
{
    arguments *threadArgs = (arguments *)arg1;
    int *count1 = (threadArgs->count1);
    vector<int> *row = threadArgs->row;
    int N = threadArgs->N;

    while (true)
    {
        while (flag.test_and_set(std::memory_order_acquire))
        {
            // wait
        }
           
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

        flag.clear(std::memory_order_release);
        // sleep(0.00001);
    }



    return NULL ;
}

int main()
{

    int N = 1024;
    int count = 0;
    int K = 16;
    vector<vector<int>> rows(K);
    arguments arg1[K];

    pthread_t thread[K];

    for (int i = 0; i < K; i++)
    {
        arg1[i].count1 = &count;
        arg1[i].row = &rows[i]; 
        arg1[i].N = N;

        if (pthread_create(thread + i, NULL, &row_assign, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }

    for (int i = 0; i < rows.size(); i++)
    {
        cout<<"Thread_id is "<<i<<endl;
        for (int j = 0; j < rows[i].size(); j++)
        {
            cout << rows[i][j] << " ";
        }
        cout <<"K" <<endl;
    }

    return 0;
}
