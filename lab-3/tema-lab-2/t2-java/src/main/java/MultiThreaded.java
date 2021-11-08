import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;
import java.util.concurrent.BrokenBarrierException;
import java.util.concurrent.CyclicBarrier;

public class MultiThreaded {
    public static void main(String[] args) {
        try {
            // reading
            File file = new File(args[0]);
            Scanner reader = new Scanner(file);
            int p = Integer.parseInt(args[1]);
            String[] words;

            int N = Integer.parseInt(reader.nextLine());
            int M = Integer.parseInt(reader.nextLine());
            int[][] F = new int[N][M];
            for (int i = 0; i < N; i++) {
                words = reader.nextLine().split(" ");
                for (int j = 0; j < M; j++) {
                    F[i][j] = Integer.parseInt(words[j]);
                }
            }

            int n = Integer.parseInt(reader.nextLine());
            int m = Integer.parseInt(reader.nextLine());
            int[][] W = new int[n][m];
            for (int i = 0; i < n; i++) {
                words = reader.nextLine().split(" ");
                for (int j = 0; j < m; j++) {
                    W[i][j] = Integer.parseInt(words[j]);
                }
            }
            reader.close();

            // multithreading init
            Thread[] threads = new Thread[p];
            CyclicBarrier barrier = new CyclicBarrier(p);
            int startI = 0, stopI, batchSize = N / p, surplus = N % p;

            // applying kernel
            Timer.Start();
            for (int i = 0; i < p; i++) {
                stopI = startI + batchSize + (surplus > 0 ? 1 : 0);
                threads[i] = new Thread(F, W, startI, stopI, N, M, n, m, barrier);
                threads[i].start();
                startI = stopI;
                surplus--;
            }

            // multithreading joining
            for (int i = 0; i < p; i++) {
                try {
                    threads[i].join();
                } catch (InterruptedException ex) {
                    ex.printStackTrace();
                }
            }
            System.out.println(Timer.Stop());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class Thread extends java.lang.Thread {
        private final int[][] F;
        private final int[][] W;
        private final int startI, stopI, N, M, n, m;
        private final CyclicBarrier barrier;

        public Thread(int[][] F, int[][] W, int startI, int stopI, int N, int M, int n, int m, CyclicBarrier barrier) {
            this.F = F;
            this.W = W;
            this.startI = startI;
            this.stopI = stopI;
            this.N = N;
            this.M = M;
            this.n = n;
            this.m = m;
            this.barrier = barrier;
        }

        @Override
        public void run() {
            try {
                // copying the needed values
                int originN = startI - (n / 2), originM = -(m / 2), sizeI = (stopI - startI) + (n - 1), sizeJ = M + (m - 1);
                int[][] aux = new int[sizeI][sizeJ];
                for (int i = originN; i < originN + sizeI; i++) {
                    for (int j = originM; j < originM + sizeJ; j++) {
                        int difI = i;
                        if (difI <= -1) difI = 0;
                        else if (difI >= N) difI = N - 1;
                        int difJ = j;
                        if (difJ <= -1) difJ = 0;
                        else if (difJ >= M) difJ = M - 1;
                        aux[i + (n / 2) - startI][j + (m / 2)] = F[difI][difJ];
                    }
                }

                // waiting for all the other threads
                barrier.await();

                // applying the filter
                int wI, wJ;
                for (int i = startI; i < stopI; i++) {
                    for (int j = 0; j < M; j++) {
                        int newValue = 0;
                        wI = 0;
                        for (int k = i - startI; k < i - startI + n; k++) {
                            wJ = 0;
                            for (int l = j; l < j + m; l++) {
                                newValue += aux[k][l] * W[wI][wJ++];
                            }
                            wI++;
                        }
                        F[i][j] = newValue;
                    }
                }
            } catch (InterruptedException | BrokenBarrierException e) {
                e.printStackTrace();
            }
        }
    }
}
