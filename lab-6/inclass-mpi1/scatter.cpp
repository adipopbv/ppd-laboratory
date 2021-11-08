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
    const int n = 18;
    int a[n], b[n], c[n];
    int *auxA = new int[n / p], *auxB = new int[n / p], *auxC = new int[n / p];
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        srand(time(NULL));
        for (int i = 0; i < n; ++i) {
            a[i] = rand() % 10;
            b[i] = rand() % 10;
        }
    }
    MPI_Scatter(a, n / p, MPI_INT, auxA, n/p, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, n / p, MPI_INT, auxB, n/p, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n / p; ++i) {
        auxC[i] = auxA[i] + auxB[i];
    }

    MPI_Gather(auxC, n / p, MPI_INT, c, n/p, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        printArray(a, n);
        printArray(b, n);
        printArray(c, n);
    }

    MPI_Finalize();

    delete[] auxA;
    delete[] auxB;
    delete[] auxC;

    return 0;
}
