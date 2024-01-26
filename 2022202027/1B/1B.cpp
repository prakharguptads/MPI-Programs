#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

bool isSafe(int row, int cols, vector<vector<int>> &board, int n)
{
    int i = row, j = cols;
    while (j >= 0)
    {
        if (board[i][j] == 1)
        {
            return false;
        }
        j--;
    }

    i = row;
    j = cols;
    while (i >= 0 && j >= 0)
    {
        if (board[i][j] == 1)
        {
            return false;
        }
        i--;
        j--;
    }

    i = row;
    j = cols;
    while (i < n && j >= 0)
    {
        if (board[i][j] == 1)
        {
            return false;
        }
        i++;
        j--;
    }

    return true;
}

int solve(vector<vector<int>> &board, int n, int cols, int &count)
{
    // base case
    if (cols >= n)
    {
        count++;
        return count;
    }
    int p = count;
    for (int row = 0; row < n; row++)
    {
        if (isSafe(row, cols, board, n))
        {
            board[row][cols] = 1;
            solve(board, n, cols + 1, count);
            board[row][cols] = 0;
        }
    }
    // cout<<count<<" cc"<<endl;
    return count;
}

int main(int argc, char *argv[])
{

    int pid, np;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int n;
    if (pid == 0)
    {
        cout << "Enter the value of N ";
        cin >> n;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // master process
    if (pid == 0)
    {
        // auto start_time = std::chrono::high_resolution_clock::now();
        int sum = 0;
        if (np == 1)
        {
            for (int i = 0; i < n; i++)
            {
                int local_count = 0;
                vector<vector<int>> board(n, vector<int>(n, 0));
                board[i][0] = 1;
                local_count = solve(board, n, 1, local_count);
                sum+=local_count;
            }
        }
        else
        {
            int i;
            int count = 0;
            for (i = 0; i < n; ++i)
            {
                // cout<<"send "<<i<<" "<<((i) % (np - 1)) + 1<<endl;
                MPI_Send(&i, 1, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
            }
            int tmp;
            for (i = 1; i < n + 1; i++)
            {
                MPI_Recv(&tmp, 1, MPI_INT,
                         MPI_ANY_SOURCE, 0,
                         MPI_COMM_WORLD,
                         &status);
                int sender = status.MPI_SOURCE;

                sum += tmp;
            }
        }

        // auto end_time = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> duration = end_time - start_time;
        // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
        cout<<sum;
    }
    // slave processes
    else
    {
        if (pid <= n)
        {
            int ll = n / (np - 1);
            if (pid <= n % (np - 1))
                ll++;
            for (int gi = 0; gi < ll; ++gi)
            {
                int cols = 0, row, local_count = 0;
                MPI_Recv(&row, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                // cout<<"rec "<<pid<<" "<<row<<" "<<ll<<endl;
                vector<vector<int>> board(n, vector<int>(n, 0));
                board[row][cols] = 1;
                // cout<<"rec "<<gi<<endl;
                local_count = solve(board, n, cols + 1, local_count);
                // cout<<"lo "<<local_count<<endl;
                MPI_Send(&local_count, 1, MPI_INT,
                         0, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();

    return 0;
}
