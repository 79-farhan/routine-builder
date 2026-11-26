#include <iostream>
#include "Scheduler.h"

void tryAdd(Scheduler& s, int id, const char* course, const char* teacher,
            const char* room, const char* section) {
    AddResult r = s.addClass(id, course, teacher, room, section);
    std::cout << "  add class " << id << ": " << addResultText(r) << "\n";
}

void tryDelete(Scheduler& s, int id) {
    DeleteResult r = s.deleteClass(id);
    std::cout << "  delete class " << id << ": " << deleteResultText(r) << "\n";
}

void tryUpdate(Scheduler& s, int id, const char* course, const char* teacher,
               const char* room, const char* section) {
    UpdateResult r = s.updateClass(id, course, teacher, room, section);
    std::cout << "  update class " << id << ": " << updateResultText(r) << "\n";
}

int main() {
    Scheduler s(5);
    int id = -1;
    PlaceResult res = PLACE_OK;

    std::cout << "Setup: add classes 1 to 4, place class 1\n";
    tryAdd(s, 1, "C1", "T1", "R1", "E");
    tryAdd(s, 2, "C2", "T2", "R2", "E");
    tryAdd(s, 3, "C3", "T3", "R3", "F");
    tryAdd(s, 4, "C4", "T4", "R4", "F");
    s.placeNext(0, 0, id, res);
    std::cout << "  class " << id << ": " << placeResultText(res) << "\n";
    s.getQueue().display();

    std::cout << "\nTest 1: delete a class that does not exist\n";
    tryDelete(s, 99);

    std::cout << "\nTest 2: delete a placed class\n";
    tryDelete(s, 1);
    std::cout << "  class 1 still placed: "
              << (s.getRoutine().isPlaced(1) ? "yes" : "no") << "\n";

    std::cout << "\nTest 3: delete a waiting class from the middle of the queue\n";
    tryDelete(s, 3);
    s.getQueue().display();
    std::cout << "  classes in list: " << s.getClasses().size() << "\n";
    std::cout << "  class 3 found: "
              << (s.getClasses().search(3) != nullptr ? "yes" : "no") << "\n";

    std::cout << "\nTest 4: a deleted id can be added again\n";
    tryAdd(s, 3, "C3", "T3", "R3", "F");
    s.getQueue().display();

    std::cout << "\nTest 5: deleting frees room under the class limit (5)\n";
    tryAdd(s, 5, "C5", "T5", "R5", "G");
    tryAdd(s, 6, "C6", "T6", "R6", "G");
    s.getQueue().display();

    std::cout << "\nTest 6: update a waiting class\n";
    tryUpdate(s, 4, "C4", "T9", "R9", "F");
    s.getClasses().display();

    std::cout << "\nTest 7: update a class that does not exist\n";
    tryUpdate(s, 99, "C1", "T1", "R1", "E");

    std::cout << "\nTest 8: update a placed class\n";
    tryUpdate(s, 1, "C1", "T7", "R7", "E");

    std::cout << "\nTest 9: update that would duplicate course and section\n";
    tryUpdate(s, 4, "C3", "T4", "R4", "F");
    ClassNode* n = s.getClasses().search(4);
    if (n != nullptr) {
        std::cout << "  class 4 is still " << n->course << " / " << n->teacher << "\n";
    }

    std::cout << "\nTest 10: update can move a waiting class to another section\n";
    tryUpdate(s, 2, "C2", "T2", "R2", "F");
    n = s.getClasses().search(2);
    if (n != nullptr) {
        std::cout << "  class 2 is now in section " << n->section << "\n";
    }

    std::cout << "\nTest 11: delete every waiting class\n";
    tryDelete(s, 2);
    tryDelete(s, 4);
    tryDelete(s, 3);
    tryDelete(s, 5);
    s.getQueue().display();
    std::cout << "  classes in list: " << s.getClasses().size() << "\n";

    std::cout << "\nTest 12: after undo the class is waiting again, so it can be deleted\n";
    UndoAction a = makePlaceAction(0, 0, 0);
    UndoResult ur = s.undoLast(a);
    std::cout << "  undo: " << (ur == UNDO_OK ? "ok" : "failed") << "\n";
    s.getQueue().display();
    tryDelete(s, 1);
    std::cout << "  classes in list: " << s.getClasses().size() << "\n";

    std::cout << "\nTest 13: nothing left to delete or undo\n";
    tryDelete(s, 1);
    ur = s.undoLast(a);
    std::cout << "  undo: " << (ur == UNDO_NOTHING ? "nothing to undo" : "unexpected") << "\n";

    return 0;
}
