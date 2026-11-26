#include "ClassQueue.h"
#include <iostream>

ClassQueue::ClassQueue(int cap) {
    if (cap < 1) {
        cap = 1;
    }
    capacity = cap;
    data = new int[capacity];
    front = 0;
    rear = 0;
    count = 0;
}

ClassQueue::~ClassQueue() {
    delete[] data;
}

// O(1): one store and one index move, no shifting.
// Returns false on overflow (queue full) and changes nothing.
bool ClassQueue::enqueue(int classId) {
    if (isFull()) {
        return false;
    }
    data[rear] = classId;
    rear = (rear + 1) % capacity;
    count++;
    return true;
}

// O(1). Returns false on underflow (queue empty).
// The removed id is handed back through the classId reference.
bool ClassQueue::dequeue(int& classId) {
    if (isEmpty()) {
        return false;
    }
    classId = data[front];
    front = (front + 1) % capacity;
    count--;
    return true;
}

// O(1). Reads the front item without removing it.
bool ClassQueue::peek(int& classId) const {
    if (isEmpty()) {
        return false;
    }
    classId = data[front];
    return true;
}

bool ClassQueue::isEmpty() const {
    return count == 0;
}

bool ClassQueue::isFull() const {
    return count == capacity;
}

int ClassQueue::size() const {
    return count;
}

// O(n): prints from front to rear, following the wrap.
void ClassQueue::display() const {
    if (isEmpty()) {
        std::cout << "Queue is empty.\n";
        return;
    }
    std::cout << "Queue (front to rear):";
    for (int i = 0; i < count; i++) {
        std::cout << " " << data[(front + i) % capacity];
    }
    std::cout << "\n";
}

// O(1): steps front back one place (wrapping below index 0) and stores the id
// there. Used by undo to put a class back exactly where it was taken from.
// Returns false on overflow (queue full) and changes nothing.
bool ClassQueue::pushFront(int classId) {
    if (isFull()) {
        return false;
    }
    front = (front - 1 + capacity) % capacity;
    data[front] = classId;
    count++;
    return true;
}

// O(n): takes every item out once and puts back all except the first match,
// so the order of the others is unchanged. Returns false if the id is not in
// the queue (also true for an empty queue). This is a maintenance operation
// for deleting a waiting class, not part of normal FIFO use.
bool ClassQueue::removeValue(int classId) {
    bool found = false;
    int n = count;
    for (int i = 0; i < n; i++) {
        int x = 0;
        dequeue(x);
        if (!found && x == classId) {
            found = true;
        } else {
            enqueue(x);
        }
    }
    return found;
}
