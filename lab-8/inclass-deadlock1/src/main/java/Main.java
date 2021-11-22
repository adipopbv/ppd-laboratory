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
            first.transfer(second, 10);
        });
        Thread t4 = new Thread(() -> {
            second.withdraw(310);
        });
        Thread t5 = new Thread(() -> {
            second.transfer(first, 120);
        });

        System.out.println(first.getAmmount());
        System.out.println(second.getAmmount());
        t1.start();
        t2.start();
        t3.start();
        t4.start();
        t5.start();

        try {
            t1.join();
            t2.join();
            t3.join();
            t4.join();
            t5.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        System.out.println(first.getAmmount());
        System.out.println(second.getAmmount());
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

    public boolean transfer(BankAccount other, double sum) {
        lock.lock();
        other.lock.lock();

        if (sum <= ammount) {
            try {
                Thread.sleep(100);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
            ammount -= sum;
            other.ammount += sum;

            lock.unlock();
            other.lock.unlock();
            return true;
        }

        lock.unlock();
        other.lock.unlock();
        return false;
    }
}

class BankAccount2 {
    double ammount;

    BankAccount2(double ammount) {
        this.ammount = ammount;
    }

    public synchronized double getAmmount() {
        return ammount;
    }

    public synchronized void deposit(double sum) {
        ammount += sum;
    }

    public synchronized boolean withdraw(double sum) {
        if (ammount >= sum) {
            ammount -= sum;

            return true;
        }

        return false;
    }

    public boolean transfer(BankAccount2 other, double sum) {
        synchronized (this) {
            synchronized (other) {
                if (sum <= ammount) {
                    ammount -= sum;
                    other.ammount += sum;

                    return true;
                }

                return false;
            }
        }
    }
}
