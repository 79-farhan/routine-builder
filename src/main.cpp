#include <iostream>
#include "UndoStack.h"

int main() {
    UndoStack st;
    UndoAction a;

    std::cout << "Test 1: pop and peek on an empty stack\n";
    if (!st.pop(a)) {
        std::cout << "  underflow caught (correct)\n";
    }
    if (!st.peek(a)) {
        std::cout << "  peek on empty caught (correct)\n";
    }

    std::cout << "\nTest 2: push three actions\n";
    st.push(makePlaceAction(1, 0, 0));
    st.push(makePlaceAction(2, 0, 1));
    st.push(makeMoveAction(1, 0, 0, 2, 3));
    st.display();
    std::cout << "  size " << st.size() << "\n";

    std::cout << "\nTest 3: pop gives the newest action first (LIFO)\n";
    if (st.pop(a)) {
        std::cout << "  popped " << (a.type == ACTION_MOVE ? "MOVE" : "PLACE")
                  << " class " << a.classId << "\n";
        std::cout << "  restore to (day " << a.fromDay << ", slot " << a.fromSlot
                  << "), currently at (day " << a.day << ", slot " << a.slot << ")\n";
    }

    std::cout << "\nTest 4: peek does not remove\n";
    if (st.peek(a)) {
        std::cout << "  top is " << (a.type == ACTION_MOVE ? "MOVE" : "PLACE")
                  << " class " << a.classId << ", size still " << st.size() << "\n";
    }

    std::cout << "\nTest 5: pop everything\n";
    while (st.pop(a)) {
        std::cout << "  popped " << (a.type == ACTION_MOVE ? "MOVE" : "PLACE")
                  << " class " << a.classId << "\n";
    }
    std::cout << "  empty: " << (st.isEmpty() ? "yes" : "no") << "\n";

    std::cout << "\nTest 6: push after the stack was emptied\n";
    st.push(makePlaceAction(7, 4, 5));
    st.display();

    std::cout << "\nTest 7: 1000 pushes on a fresh stack (no overflow)\n";
    UndoStack big;
    for (int i = 0; i < 1000; i++) {
        big.push(makePlaceAction(i, 0, 0));
    }
    std::cout << "  size after 1000 pushes: " << big.size() << "\n";
    int n = 0;
    while (big.pop(a)) {
        n++;
    }
    std::cout << "  popped " << n << " actions, empty: " << (big.isEmpty() ? "yes" : "no") << "\n";

    return 0;
}
