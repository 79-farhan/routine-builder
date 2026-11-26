#include <iostream>
#include "ClassQueue.h"

int main() {
    int x = 0;

    std::cout << "Test 1: pushFront on an empty queue\n";
    ClassQueue q(5);
    q.pushFront(10);
    q.display();
    std::cout << "  size " << q.size() << "\n";

    std::cout << "\nTest 2: pushFront puts the id ahead of everything already waiting\n";
    q.enqueue(11);
    q.enqueue(12);
    q.display();
    q.pushFront(9);
    q.display();

    std::cout << "\nTest 3: dequeue returns the id that was pushed to the front\n";
    q.dequeue(x);
    std::cout << "  got " << x << "\n";
    q.display();

    std::cout << "\nTest 4: pushFront wraps below index 0 (capacity 3)\n";
    ClassQueue w(3);
    w.pushFront(7);
    w.enqueue(8);
    w.display();
    w.pushFront(6);
    w.display();
    std::cout << "  full: " << (w.isFull() ? "yes" : "no") << "\n";

    std::cout << "\nTest 5: pushFront when full\n";
    if (!w.pushFront(5)) {
        std::cout << "  overflow caught (correct)\n";
    }

    std::cout << "\nTest 6: removeValue from the middle, front and rear\n";
    ClassQueue r(5);
    for (int i = 1; i <= 5; i++) {
        r.enqueue(i);
    }
    r.display();
    std::cout << "  remove 3: " << (r.removeValue(3) ? "found" : "not found") << "\n";
    r.display();
    std::cout << "  remove 1: " << (r.removeValue(1) ? "found" : "not found") << "\n";
    r.display();
    std::cout << "  remove 5: " << (r.removeValue(5) ? "found" : "not found") << "\n";
    r.display();
    if (!r.removeValue(99)) {
        std::cout << "  remove 99: not found (correct), queue unchanged\n";
    }
    r.display();

    std::cout << "\nTest 7: removeValue on an empty queue\n";
    ClassQueue e(3);
    if (!e.removeValue(1)) {
        std::cout << "  not found (correct)\n";
    }

    std::cout << "\nTest 8: the queue still works after removals\n";
    r.enqueue(6);
    r.enqueue(7);
    r.enqueue(8);
    r.display();
    std::cout << "  size " << r.size() << ", full: " << (r.isFull() ? "yes" : "no") << "\n";
    std::cout << "  dequeue order:";
    while (r.dequeue(x)) {
        std::cout << " " << x;
    }
    std::cout << "\n";

    return 0;
}
