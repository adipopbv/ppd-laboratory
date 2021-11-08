#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>
#include "mpi.h"

using namespace std;

void stoia(const string &stringValue, int arrayValue[]) {
    for (int i = 0; i < stringValue.size(); ++i) {
        arrayValue[i] = stringValue[i] - '0';
    }
}

void sequentialSum(const int firstNumber[], int firstNumberSize, const int secondNumber[], int secondNumberSize, int result[]) {
    int carry = 0;

    for (int i = 0; i < secondNumberSize; ++i) {
        int currentSum = (firstNumber[i] + secondNumber[i] + carry);
        carry = currentSum / 10;
        result[i] = currentSum % 10;
    }
    for (int i = secondNumberSize; i < firstNumberSize; ++i) {
        int currentSum = (firstNumber[i] + carry);
        carry = currentSum / 10;
        result[i] = currentSum % 10;
    }
    result[firstNumberSize] = carry;
}

void mpiSum(int firstNumber[], int firstNumberSize, int secondNumber[], int secondNumberSize, int result[]) {
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
    MPI_Scatter(a, n / p, MPI_INT, auxA, n / p, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(b, n / p, MPI_INT, auxB, n / p, MPI_INT, 0, MPI_COMM_WORLD);

    for (int i = 0; i < n / p; ++i) {
        auxC[i] = auxA[i] + auxB[i];
    }

    MPI_Gather(auxC, n / p, MPI_INT, c, n / p, MPI_INT, 0, MPI_COMM_WORLD);
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

int main() {
    string line;
    int firstNumberSize, secondNumberSize;
    string firstNumberStr, secondNumberStr;

    ifstream fin("../Numar1.txt");
    getline(fin, line);
    firstNumberSize = stoi(line);
    getline(fin, firstNumberStr);
    fin.close();
    fin = ifstream("../Numar2.txt");
    getline(fin, line);
    secondNumberSize = stoi(line);
    getline(fin, secondNumberStr);
    fin.close();

    if (firstNumberStr.length() < secondNumberStr.length()) {
        swap(firstNumberStr, secondNumberStr);
        swap(firstNumberSize, secondNumberSize);
    }
    reverse(firstNumberStr.begin(), firstNumberStr.end());
    reverse(secondNumberStr.begin(), secondNumberStr.end());

    int firstNumber[firstNumberSize], secondNumber[secondNumberSize], result[firstNumberSize + 1];
    stoia(firstNumberStr, firstNumber);
    stoia(secondNumberStr, secondNumber);

    sequentialSum(firstNumber, firstNumberSize, secondNumber, secondNumberSize, result);

    for (int i = firstNumberSize; i >= 0; --i) {
        if (i == firstNumberSize && result[firstNumberSize] != 0)
            cout << result[firstNumberSize];
        else
            cout << result[i];
    }
    return 0;
}
