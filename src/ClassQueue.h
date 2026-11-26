#ifndef CLASSQUEUE_H
#define CLASSQUEUE_H

// FIFO queue of class ids waiting to be placed. Fixed capacity, circular array.
class ClassQueue {
private:
    int* data;
    int capacity;
    int front;   // index of the next item to leave
    int rear;    // index where the next item will be stored
    int count;   // how many items are inside (tells full from empty)

public:
    explicit ClassQueue(int cap);
    ~ClassQueue();

    // copying would make two queues share one array and double-delete it
    ClassQueue(const ClassQueue&) = delete;
    ClassQueue& operator=(const ClassQueue&) = delete;

    bool enqueue(int classId);
    bool pushFront(int classId);
    bool dequeue(int& classId);
    bool removeValue(int classId);
    bool peek(int& classId) const;
    bool isEmpty() const;
    bool isFull() const;
    int size() const;
    void display() const;
};

#endif
