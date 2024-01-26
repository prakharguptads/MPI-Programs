#include <mpi.h>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char *argv[])
{
    int pid, np;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int n;
    int count = 0;
    if (pid == 0)
    {
        cout << "Enter the value of N: ";
        cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int graph[n][n];
    if (pid == 0)
    {
        cout << "Enter the grid: ";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> graph[i][j];
            }
        }
    }
    MPI_Bcast(graph, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (np > 1)
    {
        if (pid == 0)
        {
            // auto start_time = std::chrono::high_resolution_clock::now();
            int index, k, i;
            for (k = 0; k < n; k++)
            {
                for (i = 0; i < n; ++i)
                {
                    MPI_Send(&k, 1, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(&i, 1, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(graph[k], n, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                    MPI_Send(graph[i], n, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD);
                }
                for (i = 0; i < n; ++i)
                {
                    MPI_Recv(graph[i], n, MPI_INT, ((i) % (np - 1)) + 1, 0, MPI_COMM_WORLD, &status);
                }
            }
            // auto end_time = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> duration = end_time - start_time;
            // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    cout << graph[i][j] << " ";
                }
                cout << endl;
            }
        }
        else if (pid <= n)
        {
            int cols = 0, row = pid - 1, local_count = 0;
            int a[n], b[n], k, i;
            int ll = n / (np - 1);
            if (pid <= n % (np - 1))
                ll++;

            for (int gi = 0; gi < n * ll; ++gi)
            {
                MPI_Recv(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(a, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Recv(b, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                for (int j = 0; j < n; j++)
                {
                    if (b[k] + a[j] < b[j] && b[k] != -1 && a[j] != -1)
                        b[j] = b[k] + a[j];
                    else if (b[j] == -1 && b[k] != -1 && a[j] != -1)
                        b[j] = b[k] + a[j];
                }

                MPI_Send(b, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }
    else
    {
        // Only one process, do the computation sequentially
        if (pid == 0)
        {
            // auto start_time = std::chrono::high_resolution_clock::now();
            for (int k = 0; k < n; ++k)
            {
                for (int i = 0; i < n; ++i)
                {
                    for (int j = 0; j < n; ++j)
                    {
                        if (graph[i][k] + graph[k][j] < graph[i][j] && graph[i][k] != -1 && graph[k][j] != -1)
                            graph[i][j] = graph[i][k] + graph[k][j];
                        else if (graph[i][j] == -1 && graph[i][k] != -1 && graph[k][j] != -1)
                            graph[i][j] = graph[i][k] + graph[k][j];
                    }
                }
            }
            // auto end_time = std::chrono::high_resolution_clock::now();
            // std::chrono::duration<double> duration = end_time - start_time;
            // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    cout << graph[i][j] << " ";
                }
                cout << endl;
            }
        }
    }

    MPI_Finalize();

    return 0;
}

// #include <iostream>
// #include <vector>
// #include <limits>
// #include <mpi.h>
// #include <bits/stdc++.h>

// using namespace std;

// int main(int argc, char **argv)
// {

//     MPI_Init(&argc, &argv);

//     int size, rank, N;
//     MPI_Comm_size(MPI_COMM_WORLD, &size);
//     MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//     if (rank == 0)
//     {
//         cout << "Enter number of vertices: ";
//         cin >> N;
//     }

//     MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD);
//     int grid_matrix[N][N];

//     if (rank == 0)
//     {

//         for (int i = 0; i < N; ++i)
//         {
//             for (int j = 0; j < N; ++j)
//             {
//                 cin >> grid_matrix[i][j];
//             }
//         }
//     }

//     MPI_Bcast(grid_matrix, N * N, MPI_INT, 0, MPI_COMM_WORLD);
//     // auto start_time = std::chrono::high_resolution_clock::now();
//     for (int k = 0; k < N; k++)
//     {
//         for (int i = 0; i < N; ++i)
//         {
//             int length = N / (size);
//             if (rank <= N % (size))
//                 length++;

//             for (int l = 0; l < N * length; ++l)
//             {
//                 for (int j = 0; j < N; j++)
//                 {
//                     if (grid_matrix[i][k] + grid_matrix[k][j] < grid_matrix[i][j] && grid_matrix[i][k] != -1 && grid_matrix[k][j] != -1)
//                         grid_matrix[i][j] = grid_matrix[i][k] + grid_matrix[k][j];

//                     else if (grid_matrix[i][j] == -1 && grid_matrix[i][k] != -1 && grid_matrix[k][j] != -1)
//                         grid_matrix[i][j] = grid_matrix[i][k] + grid_matrix[k][j];
//                 }
//             }
//         }

//         MPI_Barrier(MPI_COMM_WORLD);
//         MPI_Allreduce(MPI_IN_PLACE, grid_matrix, N * N, MPI_INT, MPI_MIN, MPI_COMM_WORLD);
//     }
//     // auto end_time = std::chrono::high_resolution_clock::now();
//     // std::chrono::duration<double> duration = end_time - start_time;
//     // std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;

//     if (rank == 0)
//     {
//         for (int i = 0; i < N; i++)
//         {
//             for (int j = 0; j < N; j++)
//             {
//                 cout << grid_matrix[i][j] << " ";
//             }
//             cout << endl;
//         }
//     }
//     MPI_Finalize();
//     return 0;
// }