import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public class Main {
    public static void main(String[] args) {
        BankAccount first = new BankAccount(200), second = new BankAccount(3000);
        Thread t1 = new Thread(() -> {
            first.withdraw(10);
        });
        Thread t2 = new Thread(() -> {
            first.deposit(10);
        });
        Thread t3 = new Thread(() -> {
            first.withdraw(50);
        });
        Thread t4 = new Thread(() -> {
            first.deposit(50);
        });
        Thread t5 = new Thread(() -> {
            first.withdraw(120);
        });
        Thread t6 = new Thread(() -> {
            first.deposit(120);
        });

        System.out.println(first.getAmmount());
        t1.start();
        t2.start();
        t3.start();
        t4.start();
        t5.start();
        t6.start();

        try {
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
            t6.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(first.getAmmount());
    }
}

class BankAccount {
    double ammount;
    Lock lock = new ReentrantLock();

    BankAccount(double ammount) {
        this.ammount = ammount;
    }

    public double getAmmount() {
        return ammount;
    }

    public void deposit(double sum) {
        lock.lock();
        ammount += sum;
        lock.unlock();
    }

    public boolean withdraw(double sum) {
        lock.lock();
        if (ammount >= sum) {
            ammount -= sum;
            lock.unlock();
            return true;
        }
        lock.unlock();
        return false;
    }
}