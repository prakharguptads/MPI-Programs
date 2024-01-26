#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

void printgrid(int grid[])
{
    for (int j = 0; j < 4; j++)
    {
        cout << grid[j] << " ";
    }
    cout << endl;
    return;
}

int main(int argc, char *argv[])
{
    int pid, np;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int t, n, m;
    int h = 3;

    if (pid == 0)
    {
        cout << "Enter the value of N ";
        cin >> n;
        cout << "Enter the value of M ";
        cin >> m;
        cout << "Enter the value of T ";
        cin >> t;
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&t, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int grid[n][m];
    if (pid == 0)
    {
        cout << "Enter grid " << endl;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cin >> grid[i][j];
            }
        }
        cout << endl;
    }
    MPI_Bcast(grid, n * m, MPI_INT, 0, MPI_COMM_WORLD);

    if (pid == 0)
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < t; i++)
        {
            for (int j = 1; j < np; j++)
            {
                MPI_Send(grid, n * m, MPI_INT, j, 0, MPI_COMM_WORLD);
            }
            for (int j = 1; j < np; j++)
            {
                int temp[n][m];
                MPI_Recv(temp, n * m, MPI_INT, j, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                int ll = ceil(n / ((float)np - 1.0));
                for (int li = (j - 1) * ll; li < min((j - 1) * ll + ll, n); li++)
                {
                    for (int lj = 0; lj < m; lj++)
                    {
                        grid[li][lj] = temp[li][lj];
                    }
                }
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end_time - start_time;
        std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

        for (int j1 = 0; j1 < n; j1++)
        {
            for (int k1 = 0; k1 < m; k1++)
            {
                cout << grid[j1][k1] << " ";
            }
            cout << endl;
        }
    }
    else if (pid <= n)
    {
        for (int gi = 0; gi < t; gi++)
        {
            int h, j;
            int a0[n][m];
            MPI_Recv(a0, n * m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            int result[n][m];
            for (int i = 0; i < n; ++i)
            {
                for (int j = 0; j < m; ++j)
                {
                    result[i][j] = grid[i][j];
                }
            }
            int ll = ceil(n / ((float)np - 1.0));
            for (int i = (pid - 1) * ll; i < min((pid - 1) * ll + ll, n); i++)
            {
                for (int j = 0; j < m; j++)
                {
                    // cout<<i<<" i j "<<j<<endl;
                    int count = 0;
                    if (i > 0 && a0[i - 1][j] == 1)
                        count++;
                    if (i < n - 1 && a0[i + 1][j] == 1)
                        count++;
                    if (j < m - 1 && a0[i][j + 1] == 1)
                        count++;
                    if (j > 0 && a0[i][j - 1] == 1)
                        count++;
                    if (a0[i][j] == 1)
                    {
                        if (count == 2 || count == 3)
                            result[i][j] = 1;
                        else
                            result[i][j] = 0;
                    }
                    else if (a0[i][j] == 0 && count == 3)
                    {
                        result[i][j] = 1;
                    }
                    else
                        result[i][j] = a0[i][j];
                }
            }
            MPI_Send(result, n * m, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();
    // lifegame(grid, t);
    return 0;
}