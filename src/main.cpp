#include <iostream>
#include "Scheduler.h"

void tryAdd(Scheduler& s, int id, const char* course, const char* teacher,
            const char* room, const char* section) {
    AddResult r = s.addClass(id, course, teacher, room, section);
    std::cout << "  add class " << id << ": " << addResultText(r) << "\n";
}

void tryNext(Scheduler& s, int day, int slot) {
    int id = -1;
    PlaceResult res = PLACE_OK;
    if (!s.placeNext(day, slot, id, res)) {
        std::cout << "  place next at (day " << day << ", slot " << slot
                  << "): queue is empty\n";
        return;
    }
    std::cout << "  class " << id << " at (day " << day << ", slot " << slot
              << "): " << placeResultText(res) << "\n";
}

void tryMove(Scheduler& s, int id, int day, int slot) {
    int oldDay = -1;
    int oldSlot = -1;
    PlaceResult res = s.moveClass(id, day, slot, oldDay, oldSlot);
    std::cout << "  class " << id << " to (day " << day << ", slot " << slot << "): ";
    if (res == PLACE_OK) {
        std::cout << "moved from (day " << oldDay << ", slot " << oldSlot << ")\n";
    } else {
        std::cout << placeResultText(res) << "\n";
    }
}

void tryUndo(Scheduler& s) {
    UndoAction a = makePlaceAction(0, 0, 0);
    UndoResult r = s.undoLast(a);
    if (r == UNDO_NOTHING) {
        std::cout << "  undo: nothing to undo\n";
    } else if (r == UNDO_BROKEN) {
        std::cout << "  undo: internal error\n";
    } else if (a.type == ACTION_PLACE) {
        std::cout << "  undo: class " << a.classId << " removed from (day " << a.day
                  << ", slot " << a.slot << "), back at the front of the queue\n";
    } else {
        std::cout << "  undo: class " << a.classId << " moved back to (day "
                  << a.fromDay << ", slot " << a.fromSlot << ")\n";
    }
}

int main() {
    Scheduler s(5);
    int id = -1;

    std::cout << "Test 1: empty scheduler\n";
    tryNext(s, 0, 0);
    tryUndo(s);
    if (!s.skipNext(id)) {
        std::cout << "  skip: nothing to skip\n";
    }

    std::cout << "\nTest 2: add classes 1 to 4\n";
    tryAdd(s, 1, "C1", "T1", "R1", "E");
    tryAdd(s, 2, "C2", "T2", "R2", "E");
    tryAdd(s, 3, "C3", "T1", "R3", "F");
    tryAdd(s, 4, "C4", "T4", "R4", "F");
    s.getQueue().display();

    std::cout << "\nTest 3: duplicate id and duplicate course in the same section\n";
    tryAdd(s, 1, "C9", "T9", "R9", "G");
    tryAdd(s, 9, "C1", "T9", "R9", "E");
    s.getQueue().display();

    std::cout << "\nTest 4: fill the class limit (5), then go past it\n";
    tryAdd(s, 5, "C5", "T5", "R5", "G");
    tryAdd(s, 6, "C6", "T6", "R6", "G");
    s.getQueue().display();

    std::cout << "\nTest 5: place from the front of the queue\n";
    tryNext(s, 0, 0);
    s.getQueue().display();

    std::cout << "\nTest 6: a rejected placement leaves the class at the front\n";
    tryNext(s, 0, 0);
    s.getQueue().display();

    std::cout << "\nTest 7: place it in a free slot\n";
    tryNext(s, 0, 1);
    s.getQueue().display();

    std::cout << "\nTest 8: teacher clash keeps the class at the front\n";
    tryNext(s, 0, 0);
    s.getQueue().display();

    std::cout << "\nTest 9: skip the class that cannot be placed now\n";
    if (s.skipNext(id)) {
        std::cout << "  skipped class " << id << "\n";
    }
    s.getQueue().display();

    std::cout << "\nTest 10: place the rest, then try on an empty queue\n";
    tryNext(s, 0, 0);
    tryNext(s, 0, 2);
    tryNext(s, 1, 0);
    tryNext(s, 2, 0);

    std::cout << "\nTest 11: move a placed class\n";
    tryMove(s, 1, 0, 4);

    std::cout << "\nTest 12: the move created a gap\n";
    s.getRoutine().printGapReport();

    std::cout << "\nTest 13: rejected moves must not touch the undo stack\n";
    tryMove(s, 2, 0, 4);
    tryMove(s, 99, 0, 0);
    std::cout << "  undo stack size: " << s.getUndo().size() << "\n";

    std::cout << "\nTest 14: undo the move\n";
    tryUndo(s);
    std::cout << "  total gaps: " << s.getRoutine().totalGaps() << "\n";

    std::cout << "\nTest 15: undo two placements\n";
    tryUndo(s);
    s.getQueue().display();
    tryUndo(s);
    s.getQueue().display();

    std::cout << "\nTest 16: undo everything\n";
    tryUndo(s);
    tryUndo(s);
    tryUndo(s);
    tryUndo(s);
    s.getQueue().display();
    std::cout << "  class 1 placed: " << (s.getRoutine().isPlaced(1) ? "yes" : "no") << "\n";

    std::cout << "\nTest 17: place again after undoing everything\n";
    tryNext(s, 0, 0);
    s.getQueue().display();

    return 0;
}
