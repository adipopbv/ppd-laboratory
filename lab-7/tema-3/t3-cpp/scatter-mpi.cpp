#include <string>
#include <algorithm>
#include <iostream>
#include <fstream>

#include "timer.h"
#include "mpi.h"

using namespace std;

void stoia(const string &stringValue, int arrayValue[], int arraySize) {
    for (int i = 0; i < stringValue.size(); ++i) {
        arrayValue[i] = stringValue[i] - '0';
    }
    for (int i = (int) stringValue.size(); i < arraySize; ++i) {
        arrayValue[i] = 0;
    }
}

int main() {
    MPI_Init(nullptr, nullptr);

    int coresCount = 6;
    string line;
    string firstNumberStr, secondNumberStr;
    ifstream fin("Numar1.txt");
    getline(fin, line);
    int firstNumberSize = stoi(line);
    getline(fin, firstNumberStr);
    fin.close();
    fin = ifstream("Numar2.txt");
    getline(fin, line);
    int secondNumberSize = stoi(line);
    getline(fin, secondNumberStr);
    fin.close();

    if (firstNumberSize < secondNumberSize) {
        swap(firstNumberStr, secondNumberStr);
        swap(firstNumberSize, secondNumberSize);
    }
    int greatestNumberSize = firstNumberSize;
    if (firstNumberSize % coresCount != 0)
        greatestNumberSize += (firstNumberSize / coresCount + 1) * coresCount - firstNumberSize;
    reverse(firstNumberStr.begin(), firstNumberStr.end());
    reverse(secondNumberStr.begin(), secondNumberStr.end());

    int firstNumber[greatestNumberSize], secondNumber[greatestNumberSize], result[greatestNumberSize + 1];
    stoia(firstNumberStr, firstNumber, greatestNumberSize);
    stoia(secondNumberStr, secondNumber, greatestNumberSize);

    int p, rank;
    int carry = 0;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int *auxF = new int[greatestNumberSize / p], *auxS = new int[greatestNumberSize / p], *auxR = new int[
    greatestNumberSize / p];

    Timer timer = Timer();
    timer.start();
    MPI_Scatter(firstNumber, greatestNumberSize / p, MPI_INT, auxF, greatestNumberSize / p, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(secondNumber, greatestNumberSize / p, MPI_INT, auxS, greatestNumberSize / p, MPI_INT, 0,
                MPI_COMM_WORLD);

    // Calculate sum
    for (int i = 0; i < greatestNumberSize / p; ++i) {
        int currentSum = (auxF[i] + auxS[i] + carry);
        carry = currentSum / 10;
        auxR[i] = currentSum % 10;
    }

    if (rank != 0) {
        // Receive carry
        int rcarry;
        MPI_Recv(&rcarry, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

        // Recalculate sum
        for (int i = 0; i < greatestNumberSize / p; ++i) {
            int currentSum = auxR[i] + rcarry;
            rcarry = currentSum / 10;
            auxR[i] = currentSum % 10;
        }
        carry += rcarry;
    }

    // Send carry
    if (rank == p - 1) {
        MPI_Send(&carry, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        MPI_Send(&carry, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
    }

    MPI_Gather(auxR, greatestNumberSize / p, MPI_INT, result, greatestNumberSize / p, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        MPI_Recv(&carry, 1, MPI_INT, p - 1, 0, MPI_COMM_WORLD, &status);
        result[greatestNumberSize] = carry;
        cout << timer.stop();
        /*
        for (int i = greatestNumberSize; i >= 0; --i) {
            if (i == firstNumberSize) {
                if (result[firstNumberSize] != 0)
                    cout << result[firstNumberSize];
            } else if (i < firstNumberSize)
                cout << result[i];
        }
         */
    }

    MPI_Finalize();
    delete[] auxF;
    delete[] auxS;
    delete[] auxR;
    return 0;
}
