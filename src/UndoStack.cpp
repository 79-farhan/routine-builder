#include "UndoStack.h"
#include <iostream>

UndoAction makePlaceAction(int classId, int day, int slot) {
    UndoAction a;
    a.type = ACTION_PLACE;
    a.classId = classId;
    a.day = day;
    a.slot = slot;
    a.fromDay = -1;
    a.fromSlot = -1;
    return a;
}

UndoAction makeMoveAction(int classId, int fromDay, int fromSlot, int toDay, int toSlot) {
    UndoAction a;
    a.type = ACTION_MOVE;
    a.classId = classId;
    a.day = toDay;
    a.slot = toSlot;
    a.fromDay = fromDay;
    a.fromSlot = fromSlot;
    return a;
}

UndoStack::UndoStack() : top(nullptr), count(0) {}

UndoStack::~UndoStack() {
    StackNode* cur = top;
    while (cur != nullptr) {
        StackNode* nextNode = cur->next;  // save before deleting
        delete cur;
        cur = nextNode;
    }
}

// O(1): new node becomes the top, no walking.
void UndoStack::push(const UndoAction& action) {
    StackNode* node = new StackNode;
    node->action = action;
    node->next = top;   // old top goes underneath
    top = node;
    count++;
}

// O(1). Returns false on underflow (nothing to undo).
bool UndoStack::pop(UndoAction& action) {
    if (top == nullptr) {
        return false;
    }
    StackNode* old = top;
    action = old->action;
    top = old->next;
    delete old;
    count--;
    return true;
}

// O(1). Reads the top action without removing it.
bool UndoStack::peek(UndoAction& action) const {
    if (top == nullptr) {
        return false;
    }
    action = top->action;
    return true;
}

bool UndoStack::isEmpty() const {
    return top == nullptr;
}

int UndoStack::size() const {
    return count;
}

// O(n): prints from the top (newest) down to the bottom (oldest).
void UndoStack::display() const {
    if (top == nullptr) {
        std::cout << "Undo stack is empty.\n";
        return;
    }
    std::cout << "Undo stack (top to bottom):\n";
    StackNode* cur = top;
    while (cur != nullptr) {
        const UndoAction& a = cur->action;
        if (a.type == ACTION_PLACE) {
            std::cout << "  PLACE class " << a.classId << " at (day " << a.day
                      << ", slot " << a.slot << ")\n";
        } else {
            std::cout << "  MOVE class " << a.classId << ": (day " << a.fromDay
                      << ", slot " << a.fromSlot << ") -> (day " << a.day
                      << ", slot " << a.slot << ")\n";
        }
        cur = cur->next;
    }
}
