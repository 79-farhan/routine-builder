#include "test_helper.h"
#include "UndoStack.h"

int main() {
    UndoAction a = makePlaceAction(0, 0, 0);

    section("Empty stack");
    UndoStack st;
    check(st.isEmpty() && st.size() == 0, "new stack is empty");
    check(!st.pop(a), "pop on an empty stack fails (underflow)");
    check(!st.peek(a), "peek on an empty stack fails");

    section("Action builders");
    UndoAction p = makePlaceAction(1, 2, 3);
    check(p.type == ACTION_PLACE && p.classId == 1 && p.day == 2 && p.slot == 3,
          "place action stores class and position");
    check(p.fromDay == -1 && p.fromSlot == -1, "place action has no origin");
    UndoAction m = makeMoveAction(4, 0, 1, 2, 3);
    check(m.type == ACTION_MOVE && m.classId == 4 && m.fromDay == 0 && m.fromSlot == 1 &&
          m.day == 2 && m.slot == 3, "move action stores origin and target");

    section("LIFO order");
    st.push(makePlaceAction(1, 0, 0));
    st.push(makePlaceAction(2, 0, 1));
    st.push(makeMoveAction(1, 0, 0, 2, 3));
    check(st.size() == 3 && !st.isEmpty(), "size is 3 after three pushes");
    check(st.peek(a) && a.type == ACTION_MOVE && a.classId == 1, "peek shows the newest action");
    check(st.size() == 3, "peek does not remove anything");
    check(st.pop(a) && a.type == ACTION_MOVE && a.classId == 1 && a.fromDay == 0 &&
          a.fromSlot == 0 && a.day == 2 && a.slot == 3, "first pop returns the move");
    check(st.pop(a) && a.type == ACTION_PLACE && a.classId == 2, "second pop returns the place of class 2");
    check(st.pop(a) && a.type == ACTION_PLACE && a.classId == 1, "third pop returns the place of class 1");
    check(st.isEmpty() && !st.pop(a), "stack is empty and a further pop fails");

    section("Reuse and size");
    st.push(makePlaceAction(7, 4, 5));
    check(st.size() == 1 && st.peek(a) && a.classId == 7 && a.day == 4 && a.slot == 5,
          "push after emptying works");
    UndoStack big;
    for (int i = 0; i < 1000; i++) {
        big.push(makePlaceAction(i, 0, 0));
    }
    check(big.size() == 1000, "1000 pushes cause no overflow");
    int popped = 0;
    while (big.pop(a)) {
        popped++;
    }
    check(popped == 1000 && big.isEmpty(), "1000 pops return every action");

    return finishTests();
}
