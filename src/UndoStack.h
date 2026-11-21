#ifndef UNDOSTACK_H
#define UNDOSTACK_H

enum ActionType { ACTION_PLACE = 0, ACTION_MOVE = 1 };

// One undoable action. For PLACE: day/slot is where the class was put and
// fromDay/fromSlot are -1. For MOVE: day/slot is the new position and
// fromDay/fromSlot is the old one.
struct UndoAction {
    ActionType type;
    int classId;
    int day;
    int slot;
    int fromDay;
    int fromSlot;
};

UndoAction makePlaceAction(int classId, int day, int slot);
UndoAction makeMoveAction(int classId, int fromDay, int fromSlot, int toDay, int toSlot);

struct StackNode {
    UndoAction action;
    StackNode* next;
};

// LIFO stack of undo actions, built on a singly linked list. Top is the head.
class UndoStack {
private:
    StackNode* top;
    int count;

public:
    UndoStack();
    ~UndoStack();

    // copying would make two stacks share nodes and double-delete them
    UndoStack(const UndoStack&) = delete;
    UndoStack& operator=(const UndoStack&) = delete;

    void push(const UndoAction& action);
    bool pop(UndoAction& action);
    bool peek(UndoAction& action) const;
    bool isEmpty() const;
    int size() const;
    void display() const;
};

#endif
