#include <iostream>
#include "mpi.h"

using namespace std;

void printArray(int array[], int size) {
    for (int i = 0; i < size; ++i) {
        cout << array[i] << " ";
    }
    cout << endl;
}

int main() {
    MPI_Init(nullptr, nullptr);

    int p, rank;
    int start, end;
    const int n = 10;
    int a[n], b[n], c[n];
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < n; ++i) {
            a[i] = rand() % 10;
            b[i] = rand() % 10;
        }
        printArray(a, n);
        printArray(b, n);

        int cat = n / (p - 1), rest = n % (p - 1);
        start = 0;
        for (int i = 1; i < p; ++i) {
            end = start + cat + (rest ? 1 : 0);
            rest--;
            MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(a + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(b + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            start = end;
        }

        for (int i = 1; i < p; ++i) {
            MPI_Recv(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(c + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }

        printArray(c, n);
    }
    else {
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(a + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(b + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        for (int i = start; i < end; ++i) {
            c[i] = a[i] + b[i];
        }

        MPI_Send(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(c + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
