#include <iostream>
#include <thread>
#include <pthread.h>
#include <vector>

using namespace std;

typedef struct
{
    int row1;
    int row2;
    int N;
    vector<vector<int>> FILE *fptr; &matrix;  // Pass matrix by reference
    vector<vector<int>> &ans;     // Pass ans by reference

    // Constructor to initialize reference members
    arguments(int r1, int r2, int n, vector<vector<int>> &mat, vector<vector<int>> &a) 
        : row1(r1), row2(r2), N(n), matrix(mat), ans(a) {}
} arguments;

void *matrix_mult(void *args)
{
    arguments *threadArgs = (arguments *)args;

    int N = threadArgs->N;
    int start_row = threadArgs->row1;
    int end_row = threadArgs->row2;

    vector<vector<int>> &matrix = threadArgs->matrix;  // Use reference
    vector<vector<int>> &ans = threadArgs->ans;        // Use reference

    for (int i = start_row; i < end_row; i++)
    {
        for (int j = 0; j < N; j++)
        {
            int sum = 0;

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

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> matrix[i][j];
        }
    }

    pthread_t thread[K];
    arguments arg1[K];

    int dist = N / K;

    for (int i = 0; i < K; i++)
    {
        arg1[i] = arguments(dist * i, dist * (i + 1), N, matrix, ans);

        if (pthread_create(thread + i, NULL, &matrix_mult, (void *)&arg1[i]) != 0)
            return 2;
    }

    for (int i = 0; i < K; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
            return 3;
    }

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << ans[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}
