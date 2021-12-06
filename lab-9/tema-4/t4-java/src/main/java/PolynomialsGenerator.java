import java.io.*;
import java.util.Random;
import java.util.Scanner;

public class PolynomialsGenerator {

    public static void main(String[] args) {
        int polynomialsCount;
        int maxExponent;
        int termsCount;
        Random random = new Random();

        Scanner sc = new Scanner(System.in);
        System.out.print("number of polynomials: ");
        polynomialsCount = sc.nextInt();
        System.out.print("max exponent: ");
        maxExponent = sc.nextInt();
        System.out.print("number of terms: ");
        termsCount = sc.nextInt();

        for (int i = 0; i < polynomialsCount; i++) {
            try {
                File file = new File("./files/" + i + ".txt");
                file.createNewFile();
                FileOutputStream fos = new FileOutputStream(file);
                BufferedWriter bw = new BufferedWriter(new OutputStreamWriter(fos));

                int count = termsCount;
                int[] exponents = new int[count];
                int[] coefficients = new int[count];

                int cur = 0;
                int remaining = maxExponent;
                for (int j = 1; j <= maxExponent && count > 0; j++) {
                    double probability = random.nextDouble();
                    if (probability < ((double) count) / (double) remaining) {
                        exponents[cur] = j;
                        coefficients[cur] = random.nextInt(100) + 1;
                        count--;
                        cur++;
                    }
                    remaining--;
                }

                for (int j = 0; j < termsCount; j++) {
                    bw.write(coefficients[j] + " " + exponents[j]);
                    if (j < termsCount - 1)
                        bw.newLine();
                }
                bw.close();
                fos.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
}
