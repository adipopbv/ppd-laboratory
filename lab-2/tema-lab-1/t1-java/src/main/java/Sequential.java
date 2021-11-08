import java.io.*;
import java.util.Scanner;

public class Sequential {
    public static void main(String[] args) {
        try {
            long sum = 0;
            for (int a = 0; a < 10; a++) {
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

                // apply kernel
                int[][] V = new int[N][M];
                Timer.Start();
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
                sum += Timer.Stop();
            }
            System.out.println(sum / 10.0);
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
