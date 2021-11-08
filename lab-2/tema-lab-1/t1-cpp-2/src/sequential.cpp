#include "sequential.h"
#include "timer.h"

using namespace std;

void sequentialStatic() {
    long sum = 0;
    ifstream fin("data.txt");
    string line;
    string word;
    Timer timer = Timer();
    int N, M, n, m;

    getline(fin, line);
    N = stoi(line);
    getline(fin, line);
    M = stoi(line);
    int F[N][M];
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
    int W[n][m];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            getline(fin, word, ' ');
            W[i][j] = stoi(word);
        }
    }

    for (int a = 0; a < 10; a++) {
        // apply kernel
        int V[N][M];
        timer.start();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                V[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < m; l++) {
                        int difI = k - (n / 2) + i;
                        if (difI <= -1) difI = 0;
                        else if (difI >= N) difI = N - 1;
                        int difJ = l - (n / 2) + j;
                        if (difJ <= -1) difJ = 0;
                        else if (difJ >= M) difJ = M - 1;
                        V[i][j] += F[difI][difJ] * W[k][l];
                    }
                }
            }
        }
        sum += timer.stop();
    }
    cout << double(sum) / 10.0;
}

void sequentialDynamic() {
    long sum = 0;
    ifstream fin("data.txt");
    string line;
    string word;
    Timer timer = Timer();
    int n, m;

    getline(fin, line);
    const int N = stoi(line);
    getline(fin, line);
    const int M = stoi(line);
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
        // apply kernel
        int** V = new int*[N];
        for (int i = 0; i < N; ++i) {
            V[i] = new int[M];
        }
        timer.start();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                V[i][j] = 0;
                for (int k = 0; k < n; k++) {
                    for (int l = 0; l < m; l++) {
                        int difI = k - (n / 2) + i;
                        if (difI <= -1) difI = 0;
                        else if (difI >= N) difI = N - 1;
                        int difJ = l - (n / 2) + j;
                        if (difJ <= -1) difJ = 0;
                        else if (difJ >= M) difJ = M - 1;
                        V[i][j] += F[difI][difJ] * W[k][l];
                    }
                }
            }
        }
        sum += timer.stop();
        for (int i = 0; i < N; ++i) {
            delete[] V[i];
        }
        delete[] V;
    }
    cout << double(sum) / 10.0;

    for (int i = 0; i < N; ++i) {
        delete[] F[i];
    }
    delete[] F;
    for (int i = 0; i < n; ++i) {
        delete[] W[i];
    }
    delete[] W;
}
