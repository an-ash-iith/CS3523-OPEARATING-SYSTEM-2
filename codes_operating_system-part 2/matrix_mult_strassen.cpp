#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> matrix_mult(vector<vector<int>> matrix,vector<vector<int>> ans,int row1, int row2,int column1,int column2)
{

// base case

if((row2-row1+1)==2)
{
    
}

}

int main()
{
 int N;
    int K;


    vector<vector<int>> matrix(N, vector<int>(N));

    vector<vector<int>> ans(N, vector<int>(N));

    cout << "Enter the Matrix" << endl;

    // our matrix is the square matrix ------

    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cin >> matrix[i][j];
        }
    }

 matrix_mult(matrix,ans,0,N-1,0,N-1);

return 0;
}