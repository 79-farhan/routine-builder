#include <iostream>
#include <string>
#include "Routine.h"

void tryPlace(Routine& r, const ClassList& c, int id, int day, int slot) {
    PlaceResult res = r.place(c, id, day, slot);
    std::cout << "  class " << id << " at (day " << day << ", slot " << slot
              << "): " << placeResultText(res) << "\n";
}

void tryMove(Routine& r, const ClassList& c, int id, int day, int slot) {
    int oldDay = -1;
    int oldSlot = -1;
    PlaceResult res = r.move(c, id, day, slot, oldDay, oldSlot);
    std::cout << "  class " << id << " to (day " << day << ", slot " << slot << "): ";
    if (res == PLACE_OK) {
        std::cout << "moved from (day " << oldDay << ", slot " << oldSlot << ")\n";
    } else {
        std::cout << placeResultText(res) << "\n";
    }
}

void tryUnplace(Routine& r, int id) {
    int day = -1;
    int slot = -1;
    if (r.unplace(id, day, slot)) {
        std::cout << "  class " << id << " removed from (day " << day
                  << ", slot " << slot << ")\n";
    } else {
        std::cout << "  class " << id << " is not placed, nothing removed\n";
    }
}

int main() {
    ClassList classes;
    classes.insert(1, "CSE2103", "T1", "R1", "E");
    classes.insert(2, "MAT2101", "T2", "R2", "E");
    classes.insert(3, "PHY2101", "T3", "R3", "F");
    classes.insert(4, "CHE2101", "T1", "R4", "F");
    classes.insert(5, "ENG2101", "T5", "R1", "G");

    Routine routine;

    std::cout << "Setup: place classes 1 to 4\n";
    tryPlace(routine, classes, 1, 0, 0);
    tryPlace(routine, classes, 2, 0, 1);
    tryPlace(routine, classes, 3, 0, 0);
    tryPlace(routine, classes, 4, 0, 2);

    std::cout << "\nTest 1: unplace a placed class\n";
    tryUnplace(routine, 2);

    std::cout << "\nTest 2: unplace the same class again\n";
    tryUnplace(routine, 2);

    std::cout << "\nTest 3: unplace a class that was never placed\n";
    tryUnplace(routine, 5);

    std::cout << "\nTest 4: the freed slot can be used again\n";
    tryPlace(routine, classes, 2, 0, 1);

    std::cout << "\nTest 5: move a class to a free slot\n";
    tryMove(routine, classes, 1, 1, 3);

    std::cout << "\nTest 6: move a class that is not placed\n";
    tryMove(routine, classes, 5, 0, 0);

    std::cout << "\nTest 7: move a class to the slot it is already in\n";
    tryMove(routine, classes, 1, 1, 3);

    std::cout << "\nTest 8: move onto a slot its own section already uses\n";
    tryMove(routine, classes, 1, 0, 1);

    std::cout << "\nTest 9: move that causes a teacher clash\n";
    tryMove(routine, classes, 4, 1, 3);

    std::cout << "\nTest 10: move that causes a room clash\n";
    tryPlace(routine, classes, 5, 2, 0);
    tryMove(routine, classes, 5, 1, 3);

    std::cout << "\nTest 11: out of range and missing class\n";
    tryMove(routine, classes, 1, 9, 0);
    tryMove(routine, classes, 99, 0, 0);

    std::cout << "\nTest 12: a slot freed by a move can be reused\n";
    tryMove(routine, classes, 3, 0, 4);
    tryMove(routine, classes, 4, 0, 0);

    std::cout << "\nTest 13: final routine (rejected moves must change nothing)\n";
    routine.display(classes);

    return 0;
}
