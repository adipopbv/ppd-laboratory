#include "multiThreaded.h"
#include "timer.h"

using namespace std;

struct threadParameters {
    int F[10000][10000];
    int W[5][5];
    int V[10000][10000];
    int startI;
    int stopI;
    int N;
    int M;
    int n;
    int m;
};

void run(threadParameters params) {
    for (int i = params.startI; i < params.stopI; i++) {
        for (int j = 0; j < params.M; j++) {
            params.V[i][j] = 0;
            for (int k = 0; k < params.n; k++) {
                for (int l = 0; l < params.m; l++) {
                    int difI = k - (params.n / 2) + i;
                    if (difI == -1) difI = 0;
                    else if (difI == params.N) difI = params.N - 1;
                    int difJ = l - (params.n / 2) + j;
                    if (difJ == -1) difJ = 0;
                    else if (difJ == params.M) difJ = params.M - 1;
                    params.V[i][j] += params.F[difI][difJ] * params.W[k][l];
                }
            }
        }
    }
}

void multiThreadedStatic() {
    long sum = 0;
    ifstream fin("data.txt");
    string line;
    string word;
    Timer timer = Timer();
    int p = 0, N, M, n, m;
    cout << "p: ";
//    cin >> p;

    getline(fin, line);
    N = stoi(line);
    getline(fin, line);
    M = stoi(line);
    int F[10000][10000];
    /*
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
        threadParameters params{};
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
            startI = stopI;
            surplus--;
            params.startI = startI;
            params.stopI = stopI;
            threads[i] = thread(run, params);
        }
        for (int i = 0; i < p; i++) {
            threads[i].join();
        }
        sum += timer.stop();
    }
    cout << double(sum) / 10.0;
	*/
}

void multiThreadedDynamic() {

}
