public class Timer {
    private static long start = 0;

    public static void Start() {
        start = System.currentTimeMillis();
    }

    public static long Stop() {
        return System.currentTimeMillis() - start;
    }
}
