//
// Created by adipopbv on 10/24/21.
//

#include "timer.h"
#include "multiThreaded.h"

using namespace std;

struct staticThreadParameters {
    int F[10000][10];
    int W[5][5];
    int V[10000][10];
    int startI;
    int stopI;
    int N;
    int M;
    int n;
    int m;
};

void staticRun(staticThreadParameters params) {
    for (int i = params.startI; i < params.stopI; i++) {
        for (int j = 0; j < params.M; j++) {
            params.V[i][j] = 0;
            for (int k = 0; k < params.n; k++) {
                for (int l = 0; l < params.m; l++) {
                    int difI = k - (params.n / 2) + i;
                    if (difI <= -1) difI = 0;
                    else if (difI >= params.N) difI = params.N - 1;
                    int difJ = l - (params.n / 2) + j;
                    if (difJ <= -1) difJ = 0;
                    else if (difJ >= params.M) difJ = params.M - 1;
                    params.V[i][j] += params.F[difI][difJ] * params.W[k][l];
                }
            }
        }
    }
}

void multiThreadedStatic() {
    long sum = 0;
    ifstream fin("../data.txt");
    string line;
    string word;
    Timer timer = Timer();
    int p = 0, N, M, n, m;
    cout << "p: ";
    cin >> p;

    getline(fin, line);
    N = stoi(line);
    getline(fin, line);
    M = stoi(line);
    int F[10000][10];
    for (int i = 0; i < N; i++) {
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
    int W[5][5];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            getline(fin, word, ' ');
            W[i][j] = stoi(word);
        }
    }

    for (int a = 0; a < 10; a++) {
        thread threads[p];
        int startI = 0, stopI, batchSize = N / p, surplus = N % p;
        staticThreadParameters params{};
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                params.F[i][j] = F[i][j];
            }
        }
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                params.W[i][j] = W[i][j];
            }
        }
        params.N = N;
        params.M = M;
        params.n = n;
        params.m = m;
        // apply kernel
        timer.start();
        for (int i = 0; i < p; ++i) {
            stopI = startI + batchSize + (surplus > 0 ? 1 : 0);
            params.startI = startI;
            params.stopI = stopI;
            threads[i] = thread(staticRun, params);
            startI = stopI;
            surplus--;
        }
        for (int i = 0; i < p; i++) {
            threads[i].join();
        }
        sum += timer.stop();
    }
    cout << double(sum) / 10.0 << '\n';
}

struct dynamicThreadParameters {
    int** F;
    int** W;
    long** V;
    int startI;
    int stopI;
    int N;
    int M;
    int n;
    int m;
};

void dynamicRun(dynamicThreadParameters params) {
    for (int i = params.startI; i < params.stopI; i++) {
        for (int j = 0; j < params.M; j++) {
            params.V[i][j] = 0;
            for (int k = 0; k < params.n; k++) {
                for (int l = 0; l < params.m; l++) {
                    int difI = k - (params.n / 2) + i;
                    if (difI <= -1) difI = 0;
                    else if (difI >= params.N) difI = params.N - 1;
                    int difJ = l - (params.n / 2) + j;
                    if (difJ <= -1) difJ = 0;
                    else if (difJ >= params.M) difJ = params.M - 1;
                    params.V[i][j] += params.F[difI][difJ] * params.W[k][l];
                }
            }
        }
    }
}

void multiThreadedDynamic() {
    long sum = 0;
    ifstream fin("../data.txt");
    string line;
    string word;
    Timer timer = Timer();
    int p = 0, N, M, n, m;
    cout << "p: ";
    cin >> p;

    getline(fin, line);
    N = stoi(line);
    getline(fin, line);
    M = stoi(line);
    int** F = new int*[N];
    for (int i = 0; i < N; ++i) {
        F[i] = new int[M];
    }
    for (int i = 0; i < N; i++) {
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
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            getline(fin, word, ' ');
            W[i][j] = stoi(word);
        }
    }

    for (int a = 0; a < 10; a++) {
        thread threads[p];
        int startI = 0, stopI, batchSize = N / p, surplus = N % p;
        dynamicThreadParameters params{};
        params.F = F;
        params.W = W;
        long** V = new long*[N];
        for (int i = 0; i < N; ++i) {
            V[i] = new long[M];
        }
        params.V = V;
        params.N = N;
        params.M = M;
        params.n = n;
        params.m = m;
        // apply kernel
        timer.start();
        for (int i = 0; i < p; ++i) {
            stopI = startI + batchSize + (surplus > 0 ? 1 : 0);
            params.startI = startI;
            params.stopI = stopI;
            threads[i] = thread(dynamicRun, params);
            startI = stopI;
            surplus--;
        }
        for (int i = 0; i < p; i++) {
            threads[i].join();
        }
        sum += timer.stop();

        for (int i = 0; i < N; ++i) {
            delete[] V[i];
        }
        delete[] V;
    }
    cout << double(sum) / 10.0 << '\n';

    for (int i = 0; i < N; ++i) {
        delete[] F[i];
    }
    delete[] F;
    for (int i = 0; i < n; ++i) {
        delete[] W[i];
    }
    delete[] W;
}
