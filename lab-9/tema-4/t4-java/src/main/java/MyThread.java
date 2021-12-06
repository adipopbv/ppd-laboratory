public class MyThread extends Thread {
    @Override
    public void run() {
        Term term;
        synchronized (Main.termsQueue) {
            term = Main.termsQueue.pop();
            if (term == null)
                return;
        }
        while (term != null) {
            synchronized (Main.polynomial) {
                Main.polynomial.addTerm(new Node(term));
            }
            synchronized (Main.termsQueue) {
                term = Main.termsQueue.pop();
            }
        }
    }
}
