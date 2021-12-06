import java.util.ArrayList;
import java.util.List;

public class Queue {
    List<Term> values = new ArrayList<>();

    void add(Term term) {
        values.add(term);
    }

    Term pop() {
        if (!values.isEmpty())
            return values.remove(0);
        return null;
    }

    boolean isEmpty() {
        return values.isEmpty();
    }
}
