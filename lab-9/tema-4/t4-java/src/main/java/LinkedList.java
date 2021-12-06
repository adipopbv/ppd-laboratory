import java.util.ArrayList;
import java.util.List;

public class LinkedList {
    public Node root = null;

    public void addTerm(Node newNode) {
        // empty list
        if (root == null) {
            root = newNode;
            return;
        }
        // must be placed in root
        if (newNode.value.exponent > root.value.exponent) {
            newNode.nextNode = root;
            root = newNode;
            return;
        }
        // must be added to the root node
        if (newNode.value.exponent == root.value.exponent) {
            root.value.coefficient += newNode.value.coefficient;
            return;
        }
        Node previousNode = root, currentNode = root.nextNode;
        while (currentNode != null) {
            // must be placed somewhere not at the end
            if (newNode.value.exponent > currentNode.value.exponent) {
                newNode.nextNode = currentNode;
                previousNode.nextNode = newNode;
                return;
            }
            // must be added to a node somewhere
            if (newNode.value.exponent == currentNode.value.exponent) {
                currentNode.value.coefficient += newNode.value.coefficient;
                return;
            }
            previousNode = currentNode;
            currentNode = currentNode.nextNode;
        }
        // must be placed at the end
        previousNode.nextNode = newNode;
    }

    List<Term> asList() {
        List<Term> terms = new ArrayList<>();
        Node currentNode = root;
        while (currentNode != null) {
            terms.add(currentNode.value);
            currentNode = currentNode.nextNode;
        }
        return terms;
    }
}
