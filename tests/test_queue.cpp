#include "test_helper.h"
#include "ClassQueue.h"

int main() {
    int x = -1;

    section("Empty queue");
    ClassQueue q(5);
    check(q.isEmpty() && q.size() == 0, "new queue is empty");
    check(!q.dequeue(x), "dequeue on an empty queue fails (underflow)");
    check(!q.peek(x), "peek on an empty queue fails");
    check(!q.removeValue(1), "removeValue on an empty queue returns false");

    section("FIFO order and overflow");
    for (int i = 1; i <= 5; i++) {
        q.enqueue(i);
    }
    check(q.isFull() && q.size() == 5, "queue is full after five enqueues");
    check(!q.enqueue(99), "enqueue on a full queue fails (overflow)");
    check(q.size() == 5, "failed enqueue does not change the size");
    check(q.dequeue(x) && x == 1, "first dequeue returns 1");
    check(q.dequeue(x) && x == 2, "second dequeue returns 2");
    check(q.enqueue(6) && q.enqueue(7) && q.isFull(), "enqueue wraps around and fills the queue");
    check(q.peek(x) && x == 3 && q.size() == 5, "peek returns the front without removing it");
    int expected[5] = {3, 4, 5, 6, 7};
    bool orderOk = true;
    for (int i = 0; i < 5; i++) {
        if (!q.dequeue(x) || x != expected[i]) {
            orderOk = false;
        }
    }
    check(orderOk, "items leave in FIFO order across the wrap");
    check(q.isEmpty(), "queue is empty after all dequeues");
    check(q.enqueue(42) && q.peek(x) && x == 42, "queue is reusable after emptying");

    section("pushFront");
    ClassQueue w(3);
    check(w.pushFront(7), "pushFront on an empty queue");
    check(w.enqueue(8), "enqueue after pushFront");
    check(w.pushFront(6) && w.isFull(), "pushFront wraps below index 0 and fills the queue");
    check(!w.pushFront(5), "pushFront on a full queue fails");
    bool pfOrder = w.dequeue(x) && x == 6 && w.dequeue(x) && x == 7 &&
                   w.dequeue(x) && x == 8;
    check(pfOrder, "pushed-front item leaves first, then the rest in order");

    section("removeValue");
    ClassQueue r(5);
    for (int i = 1; i <= 5; i++) {
        r.enqueue(i);
    }
    check(r.removeValue(3), "remove from the middle");
    check(r.removeValue(1), "remove from the front");
    check(r.removeValue(5), "remove from the rear");
    check(!r.removeValue(99), "removing a missing id returns false");
    check(r.size() == 2, "size is 2 after three removals and one miss");
    check(r.dequeue(x) && x == 2 && r.dequeue(x) && x == 4 && r.isEmpty(),
          "remaining items keep their order");
    ClassQueue f(3);
    f.enqueue(1);
    f.enqueue(2);
    f.enqueue(3);
    check(f.isFull() && f.removeValue(2) && f.size() == 2, "removeValue works on a full queue");
    check(f.dequeue(x) && x == 1 && f.dequeue(x) && x == 3, "order is kept after removing from a full queue");

    section("Capacity edge case");
    ClassQueue tiny(0);
    check(tiny.enqueue(1) && !tiny.enqueue(2), "a capacity below 1 is treated as 1");

    return finishTests();
}
