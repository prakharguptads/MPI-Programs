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
    if (np > 1)
    {
        if (pid == 0)
        {
            // auto start_time = std::chrono::high_resolution_clock::now();
            for (int i = 0; i < t; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    MPI_Send(&j, 1, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    if (j == 0)
                    {
                        MPI_Send(grid[j], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(grid[j + 1], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    }
                    else if (j == n - 1)
                    {
                        MPI_Send(grid[j - 1], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(grid[j], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    }
                    else
                    {
                        MPI_Send(grid[j - 1], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(grid[j], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(grid[j + 1], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    }
                }
                for (int j = 0; j < n; j++)
                {
                    MPI_Recv(grid[j], m, MPI_INT, ((j) % (np - 1)) + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
            // auto end_time = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> duration = end_time - start_time;
            // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

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
            int ll = n / (np - 1);
            if (pid <= n % (np - 1))
                ll++;
            for (int gi = 0; gi < ll * t; gi++)
            {
                int h, j;
                int a0[m], a1[m], a2[m];
                int result[m];
                MPI_Recv(&j, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(a0, m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(a1, m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                if (j != 0 && j != n - 1)
                {
                    MPI_Recv(a2, m, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
                if (j == 0)
                {
                    for (int k = 0; k < m; k++)
                    {
                        int count = 0;
                        if (a1[k] == 1)
                            count++;
                        if (k > 0 && a0[k - 1] == 1)
                            count++;
                        if (k > 0 && a1[k - 1] == 1)
                            count++;
                        if (k < m - 1 && a0[k + 1] == 1)
                            count++;
                        if (k < m - 1 && a1[k + 1] == 1)
                            count++;
                        if (a0[k] == 1)
                        {
                            if (count < 2)
                            {
                                result[k] = 0;
                            }
                            else if (count == 2 || count == 3)
                            {
                                result[k] = 1;
                            }
                            else
                            {
                                result[k] = 0;
                            }
                        }
                        else if (a0[k] == 0 && count == 3)
                        {
                            result[k] = 1;
                        }
                        else
                        {
                            result[k] = a0[k];
                        }
                    }
                    MPI_Send(result, m, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
                else if (j == n - 1)
                {
                    for (int k = 0; k < m; k++)
                    {
                        int count = 0;
                        if (a0[k] == 1)
                            count++;
                        if (k > 0 && a1[k - 1] == 1)
                            count++;
                        if (k > 0 && a0[k - 1] == 1)
                            count++;
                        if (k < m - 1 && a1[k + 1] == 1)
                            count++;
                        if (k < m - 1 && a0[k + 1] == 1)
                            count++;
                        if (a1[k] == 1)
                        {
                            if (count < 2)
                            {
                                result[k] = 0;
                            }
                            else if (count == 2 || count == 3)
                            {
                                result[k] = 1;
                            }
                            else
                            {
                                result[k] = 0;
                            }
                        }
                        else if (a1[k] == 0 && count == 3)
                        {
                            result[k] = 1;
                        }
                        else
                        {
                            result[k] = a1[k];
                        }
                    }
                    MPI_Send(result, m, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
                else
                {
                    for (int k = 0; k < m; k++)
                    {
                        int count = 0;
                        if (a0[k] == 1)
                            count++;
                        if (a2[k] == 1)
                            count++;
                        if (k > 0 && a1[k - 1] == 1)
                            count++;
                        if (k > 0 && a2[k - 1] == 1)
                            count++;
                        if (k > 0 && a0[k - 1] == 1)
                            count++;
                        if (k < m - 1 && a1[k + 1] == 1)
                            count++;
                        if (k < m - 1 && a2[k + 1] == 1)
                            count++;
                        if (k < m - 1 && a0[k + 1] == 1)
                            count++;
                        if (a1[k] == 1)
                        {
                            if (count < 2)
                            {
                                result[k] = 0;
                            }
                            else if (count == 2 || count == 3)
                            {
                                result[k] = 1;
                            }
                            else
                            {
                                result[k] = 0;
                            }
                        }
                        else if (a1[k] == 0 && count == 3)
                        {
                            result[k] = 1;
                        }
                        else
                        {
                            result[k] = a1[k];
                        }
                    }
                    MPI_Send(result, m, MPI_INT, 0, 0, MPI_COMM_WORLD);
                }
            }
        }
    }
    else
    {
        if (pid == 0)
        {
            for (int i = 0; i < t; i++)
            {
                int temp[n][m];
                for (int j = 0; j < n; j++)
                {
                    for (int k = 0; k < m; k++)
                    {
                        int count = 0;
                        if (j > 0 && grid[j - 1][k] == 1)
                            count++;
                        if (j > 0 && k > 0 && grid[j - 1][k - 1] == 1)
                            count++;
                        if (j > 0 && k < m - 1 && grid[j - 1][k + 1] == 1)
                            count++;
                        if (k > 0 && grid[j][k - 1] == 1)
                            count++;
                        if (j < n - 1 && k > 0 && grid[j + 1][k - 1] == 1)
                            count++;
                        if (j < n - 1 && k < m - 1 && grid[j + 1][k + 1] == 1)
                            count++;
                        if (k < m - 1 && grid[j][k + 1] == 1)
                            count++;
                        if (j < n - 1 && grid[j + 1][k] == 1)
                            count++;
                        if (grid[j][k] == 1)
                        {
                            if (count < 2)
                            {
                                temp[j][k] = 0;
                            }
                            else if (count == 2 || count == 3)
                            {
                                temp[j][k] = 1;
                            }
                            else
                            {
                                temp[j][k] = 0;
                            }
                        }
                        else if (grid[j][k] == 0 && count == 3)
                        {
                            temp[j][k] = 1;
                        }
                        else
                        {
                            temp[j][k] = grid[j][k];
                        }
                    }
                }
                for (int j = 0; j < n; j++)
                {
                    for (int k = 0; k < m; k++)
                    {
                        grid[j][k] = temp[j][k];
                    }
                }
            }
            for (int j = 0; j < n; j++)
            {
                for (int k = 0; k < m; k++)
                {
                    cout << grid[j][k] << " ";
                }
                cout << endl;
            }
        }
    }
    MPI_Finalize();
    // lifegame(grid, t);
    return 0;
}