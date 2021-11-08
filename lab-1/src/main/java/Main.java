import java.util.Arrays;
import java.util.Random;

public class Main {
    public static void main(String[] args) {
        int n = 100000000;
        int[] a = new int[n];

        Random r = new Random();
        for (int i = 0; i < n; i++) {
            a[i] = r.nextInt(10);
        }

        int[] b = new int[n];
        for (int i = 0; i < n; i++) {
            b[i] = r.nextInt(10);
        }

//        System.out.println(Arrays.toString(a));
//        System.out.println(Arrays.toString(b));

        long start, end;
        double[] c = new double[n];
        start = System.currentTimeMillis();
        for (int i = 0; i < n; i++) {
            c[i] = a[i] + b[i];
        }
        end = System.currentTimeMillis();

//        System.out.println(Arrays.toString(c));
        System.out.println(end - start);

        // n = 10, p = 4, [n/p] = 2, n%p = 2
        // Thr 1: 3 mere
        // Thr 2: 3 mere
        // Thr 3: 2 mere
        // Thr 4: 2 mere

        int p = 4;
        int s = 0, e;
        int cat = n / p, rest = n % p;
        MyThread[] threads = new MyThread[p];
        start = System.currentTimeMillis();
        for (int i = 0; i < p; i++) {
            e = s + cat + (rest > 0 ? 1 : 0);
            rest--;
//            System.out.println(s + " " + e);
            s = e;

            threads[i] = new MyThread(s, e, a, b, c);
            threads[i].start();
        }
        for (int i = 0; i < p; i++) {
            try {
                threads[i].join();
            } catch (InterruptedException ex) {
                ex.printStackTrace();
            }
        }
        end = System.currentTimeMillis();
        System.out.println(end - start);

    }

    public static class MyThread extends Thread {
        private final int left, right;
        private final int[] a, b;
        private final double[] c;

        public MyThread(int left, int right, int[] a, int[] b, double[] c) {
            this.left = left;
            this.right = right;
            this.a = a;
            this.b = b;
            this.c = c;
        }

        @Override
        public void run() {
//            for (int i = left; i < right; i++) {
//                c[i] = a[i] + b[i];
//            }

            for (int i = left; i < right; i++) {
                c[i] = Math.sqrt(a[i] * a[i] * a[i] * a[i] * a[i] * b[i] * b[i] * b[i]);
            }
        }
    }
}
