#include <mpi.h>
#include <iostream>
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