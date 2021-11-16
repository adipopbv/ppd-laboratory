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
    reverse(firstNumberStr.begin(), firstNumberStr.end());
    reverse(secondNumberStr.begin(), secondNumberStr.end());

    int firstNumber[greatestNumberSize], secondNumber[greatestNumberSize], result[greatestNumberSize + 1];
    stoia(firstNumberStr, firstNumber, greatestNumberSize);
    stoia(secondNumberStr, secondNumber, greatestNumberSize);

    int p, rank;
    int start, end, carry;
    MPI_Status status;

    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == 0) {
        int cat = greatestNumberSize / (p - 1), rest = greatestNumberSize % (p - 1);
        start = 0;
        Timer timer = Timer();
        timer.start();
        for (int i = 1; i < p; ++i) {
            end = start + cat + (rest > 0 ? 1 : 0);
            rest--;
            MPI_Send(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(firstNumber + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(secondNumber + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD);
            start = end;
        }
        for (int i = 1; i < p; ++i) {
            MPI_Recv(&start, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&end, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(result + start, end - start, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        cout << timer.stop();

        /*
        for (int i = greatestNumberSize; i >= 0; --i) {
            if (i == greatestNumberSize) {
                if (result[greatestNumberSize] != 0)
                    cout << result[greatestNumberSize];
            } else
                cout << result[i];
        }
         */
    } else {
        MPI_Recv(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(firstNumber + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(secondNumber + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

        // Calculate sum
        carry = 0;
        for (int i = start; i < end; ++i) {
            int currentSum = (firstNumber[i] + secondNumber[i] + carry);
            carry = currentSum / 10;
            result[i] = currentSum % 10;
        }

        if (rank != 1) {
            // Receive carry
            int rcarry;
            MPI_Recv(&rcarry, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);

            // Recalculate sum
            for (int i = start; i < end; ++i) {
                int currentSum = result[i] + rcarry;
                rcarry = currentSum / 10;
                result[i] = currentSum % 10;
            }
            carry += rcarry;
        }

        // Send carry
        if (rank != p - 1) {
            MPI_Send(&carry, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }

        if (rank == p - 1) {
            result[greatestNumberSize] = carry;
            end++;
        }
        MPI_Send(&start, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(result + start, end - start, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
