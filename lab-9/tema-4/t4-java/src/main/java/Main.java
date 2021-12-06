import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Scanner;


public class Main {
    static final Queue termsQueue = new Queue();
    static final LinkedList polynomial = new LinkedList();

    public static void main(String[] args) {
        Timer.Start();

        int polynomialsCount = new File("./" + args[0] + "/").list().length;

        for (int i = 0; i < polynomialsCount; i++) {
            try {
                File file = new File("./" + args[0] + "/" + i + ".txt");
                Scanner reader = new Scanner(file);
                while (reader.hasNextLine()) {
                    String line = reader.nextLine();
                    String[] words = line.split(" ");
                    termsQueue.add(new Term(Integer.parseInt(words[0]), Integer.parseInt(words[1])));
                }

                int p = Integer.parseInt(args[1]);
                MyThread[] threads = new MyThread[p];
                for (int j = 0; j < p; j++) {
                    MyThread thread = new MyThread();
                    thread.start();
                    threads[j] = thread;
                }
                for (MyThread thread: threads) {
                    try {
                        thread.join();
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                }

            } catch (FileNotFoundException e) {
                e.printStackTrace();
            }
        }

        try {
            StringBuilder output = new StringBuilder();
            FileWriter myWriter = new FileWriter("out.txt");
            for (Term term: polynomial.asList()) {
                output.append(term.coefficient).append(" ").append(term.exponent).append("\n");
            }
            myWriter.write(output.toString());
            myWriter.close();
        } catch (IOException e) {
            e.printStackTrace();
        }

        System.out.println(Timer.Stop());
    }
}

