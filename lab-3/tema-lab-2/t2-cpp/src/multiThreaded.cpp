#include "multiThreaded.h"
#include "timer.h"

using namespace std;

struct threadParameters {
    int** F;
    int** W;
    int startI;
    int stopI;
    int N;
    int M;
    int n;
    int m;
    barrier* myBarrier;
};

void run(threadParameters params) {
    int originN = params.startI - (params.n / 2), originM = -(params.m / 2), sizeI = (params.stopI - params.startI) + (params.n - 1), sizeJ = params.M + (params.m - 1);
    int** aux = new int*[sizeI];
    for (int i = 0; i < sizeI; ++i) {
        aux[i] = new int[sizeJ];
    }
    for (int i = originN; i < originN + sizeI; i++) {
        for (int j = originM; j < originM + sizeJ; j++) {
            int difI = i;
            if (difI <= -1) difI = 0;
            else if (difI >= params.N) difI = params.N - 1;
            int difJ = j;
            if (difJ <= -1) difJ = 0;
            else if (difJ >= params.M) difJ = params.M - 1;
            aux[i + (params.n / 2) - params.startI][j + (params.m / 2)] = params.F[difI][difJ];
        }
    }

    // waiting for all the other threads
    params.myBarrier->wait();

    // applying the filter
    int wI, wJ;
    for (int i = params.startI; i < params.stopI; i++) {
        for (int j = 0; j < params.M; j++) {
            int newValue = 0;
            wI = 0;
            for (int k = i - params.startI; k < i - params.startI + params.n; k++) {
                wJ = 0;
                for (int l = j; l < j + params.m; l++) {
                    newValue += aux[k][l] * params.W[wI][wJ++];
                }
                wI++;
            }
            params.F[i][j] = newValue;
        }
    }

    // memory cleanup
    for (int i = 0; i < sizeI; ++i) {
        delete[] aux[i];
    }
    delete[] aux;
}

void multiThreaded(int argc, char **argv) {
    ifstream fin(argv[1]);
    string line;
    string word;
    Timer timer = Timer();
    int p = stoi(argv[2]), N, M, n, m;

    // reading
    getline(fin, line);
    N = stoi(line);
    getline(fin, line);
    M = stoi(line);
    int** F = new int*[N];
    for (int i = 0; i < N; ++i) {
        F[i] = new int[M];
        for (int j = 0; j < M; j++) {
            getline(fin, word, ' ');
            F[i][j] = stoi(word);
        }
    }

    getline(fin, line);
    getline(fin, line);
    n = stoi(line);
    getline(fin, line);
    m = stoi(line);
    int** W = new int*[n];
    for (int i = 0; i < n; ++i) {
        W[i] = new int[m];
        for (int j = 0; j < m; ++j) {
            getline(fin, word, ' ');
            W[i][j] = stoi(word);
        }
    }

    // multithreading init
    thread threads[p];
    int startI = 0, stopI, batchSize = N / p, surplus = N % p;
    threadParameters params{};
    params.F = F;
    params.W = W;
    params.N = N;
    params.M = M;
    params.n = n;
    params.m = m;
    params.myBarrier = new barrier(p);

    // apply kernel
    timer.start();
    for (int i = 0; i < p; ++i) {
        stopI = startI + batchSize + (surplus > 0 ? 1 : 0);
        params.startI = startI;
        params.stopI = stopI;
        threads[i] = thread(run, params);
        startI = stopI;
        surplus--;
    }

    // multithreading joining
    for (int i = 0; i < p; i++) {
        threads[i].join();
    }

    // memory cleanup
    for (int i = 0; i < N; ++i) {
        delete[] F[i];
    }
    delete[] F;
    for (int i = 0; i < n; ++i) {
        delete[] W[i];
    }
    delete[] W;

    cout << timer.stop();
}