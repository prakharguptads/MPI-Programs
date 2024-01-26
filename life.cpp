#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <fstream>
#include <bits/stdc++.h>
#include <iomanip>
using namespace std;

int main(int argc, char *argv[])
{

    int pid, np, elements_per_process, n_elements_recieved;

    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &pid);
    MPI_Comm_size(MPI_COMM_WORLD, &np);

    int n;
    int count = 0;
    if (pid == 0)
    {
        cout<<"Enter the value of N ";
        cin >> n;
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int graph[n][n];
    if (pid == 0)
    {
        cout<<"Enter grid ";
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                cin >> graph[i][j];
            }
        }
        // cout << endl;
    }
    MPI_Bcast(graph, n * n, MPI_INT, 0, MPI_COMM_WORLD);

    if (pid == 0)
    {
        // auto start_time = std::chrono::high_resolution_clock::now();
        int index, k, i;
        for (k = 0; k < n; k++)
        {
            for (i = 0; i < n; ++i)
            {
                MPI_Send(&k, 1, MPI_INT, ((i) % (np)), 0, MPI_COMM_WORLD);
                MPI_Send(&i, 1, MPI_INT, ((i) % (np)), 0, MPI_COMM_WORLD);
                MPI_Send(graph[k], n, MPI_INT, ((i) % (np)), 0, MPI_COMM_WORLD);
                MPI_Send(graph[i], n, MPI_INT, ((i) % (np)), 0, MPI_COMM_WORLD);
            }
            for (i = 0; i < n; ++i)
            {
                MPI_Recv(graph[i], n, MPI_INT, ((i) % (np )), 0, MPI_COMM_WORLD, &status);
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
        int cols = 0, row = pid , local_count = 0;
        int a[n], b[n], k, i;
        int ll = n / (np);
        if (pid <= n % (np))
            ll++;
        for (int gi = 0; gi < n * ll; ++gi)
        {
            MPI_Recv(&k, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(a, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(b, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            for (int j = 0; j < n; j++)
            {
                if (b[k] + a[j] < b[j] && b[k]!=-1 && a[j]!=-1)
                    b[j] = b[k] + a[j];
                else if(b[j] == -1 && b[k]!=-1 && a[j]!=-1)
                    b[j] = b[k] + a[j];
            }

            MPI_Send(b, n, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
    MPI_Finalize();

    return 0;
}
