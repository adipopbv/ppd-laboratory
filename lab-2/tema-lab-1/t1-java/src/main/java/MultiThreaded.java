import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

public class MultiThreaded {
    public static void main(String[] args) {
        try {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("p: ");
            int p = Integer.parseInt(br.readLine());

            long sum = 0;
            for (int a = 0; a < 10; a++) {
                // reading
                File file = new File("data.txt");
                Scanner reader = new Scanner(file);
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
                int[][] V = new int[N][M];
                int startI = 0, stopI, batchSize = N / p, surplus = N % p;

                // applying kernel
                Timer.Start();
                for (int i = 0; i < p; i++) {
                    stopI = startI + batchSize + (surplus > 0 ? 1 : 0);
                    threads[i] = new Thread(F, W, V, startI, stopI, N, M, n, m);
                    threads[i].start();
                    startI = stopI;
                    surplus--;
                }
                for (int i = 0; i < p; i++) {
                    try {
                        threads[i].join();
                    } catch (InterruptedException ex) {
                        ex.printStackTrace();
                    }
                }
                sum += Timer.Stop();
            }
            System.out.println(sum / 10.0);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private static class Thread extends java.lang.Thread {
        private final int[][] F;
        private final int[][] W;
        private final int[][] V;
        private final int startI, stopI, N, M, n, m;

        public Thread(int[][] F, int[][] W, int[][] V, int startI, int stopI, int N, int M, int n, int m) {
            this.F = F;
            this.W = W;
            this.V = V;
            this.startI = startI;
            this.stopI = stopI;
            this.N = N;
            this.M = M;
            this.n = n;
            this.m = m;
        }

        @Override
        public void run() {
            for (int i = startI; i < stopI; i++) {
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
        }
    }
}
