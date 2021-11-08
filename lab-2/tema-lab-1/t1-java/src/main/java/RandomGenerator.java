import java.io.*;
import java.util.Random;

public class RandomGenerator {
    public static void main(String[] args) {
        try {
            Random rand = new Random();
            StringBuilder stringBuilder = new StringBuilder();

            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            System.out.print("N: ");
            int N = Integer.parseInt(br.readLine());
            System.out.print("M: ");
            int M = Integer.parseInt(br.readLine());
            System.out.print("n: ");
            int n = Integer.parseInt(br.readLine());
            System.out.print("m: ");
            int m = Integer.parseInt(br.readLine());

            stringBuilder.append(N).append("\n");
            stringBuilder.append(M).append("\n");
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < M; j++) {
                    stringBuilder.append(rand.nextInt(100) + 1).append(" ");
                }
                stringBuilder.append("\n");
            }
            stringBuilder.append(n).append("\n");
            stringBuilder.append(m).append("\n");
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < m; j++) {
                    stringBuilder.append((rand.nextInt(5) + 1)).append(" ");
                }
                stringBuilder.append("\n");
            }

            BufferedWriter writer = new BufferedWriter(new FileWriter("data.txt"));
            writer.write(stringBuilder.toString());
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
