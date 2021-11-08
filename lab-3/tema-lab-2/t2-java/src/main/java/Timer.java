public class Timer {
    private static long start = 0;

    public static void Start() {
        start = System.currentTimeMillis();
    }

    public static long Stop() {
        return System.currentTimeMillis() - start;
    }

    public static void PrintState() {
        System.out.println(System.currentTimeMillis() - start);
    }

    public static void PrintPeriod() {
        long stop = System.currentTimeMillis();
        System.out.println(stop - start);
        start = stop;
    }
}
