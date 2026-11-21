#include <iostream>
#include "ClassQueue.h"

int main() {
    ClassQueue q(5);
    int x;

    std::cout << "Test 1: dequeue and peek on an empty queue\n";
    if (!q.dequeue(x)) {
        std::cout << "  underflow caught (correct)\n";
    }
    if (!q.peek(x)) {
        std::cout << "  peek on empty caught (correct)\n";
    }

    std::cout << "\nTest 2: enqueue 1 to 5 (fills the queue)\n";
    for (int i = 1; i <= 5; i++) {
        q.enqueue(i);
    }
    q.display();
    std::cout << "  size " << q.size() << ", full: " << (q.isFull() ? "yes" : "no") << "\n";

    std::cout << "\nTest 3: enqueue when full\n";
    if (!q.enqueue(99)) {
        std::cout << "  overflow caught (correct)\n";
    }

    std::cout << "\nTest 4: FIFO order, dequeue two\n";
    q.dequeue(x);
    std::cout << "  got " << x << "\n";
    q.dequeue(x);
    std::cout << "  got " << x << "\n";
    q.display();

    std::cout << "\nTest 5: wraparound, enqueue 6 and 7\n";
    q.enqueue(6);
    q.enqueue(7);
    q.display();
    std::cout << "  size " << q.size() << "\n";

    std::cout << "\nTest 6: peek does not remove\n";
    q.peek(x);
    std::cout << "  front is " << x << ", size still " << q.size() << "\n";

    std::cout << "\nTest 7: empty it completely\n";
    while (q.dequeue(x)) {
        std::cout << "  got " << x << "\n";
    }
    std::cout << "  empty: " << (q.isEmpty() ? "yes" : "no") << "\n";

    std::cout << "\nTest 8: reuse after emptying\n";
    q.enqueue(42);
    q.display();

    return 0;
}
