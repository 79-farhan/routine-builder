#include <iostream>
#include <string>
#include "Routine.h"

void tryPlace(Routine& r, const ClassList& c, int id, int day, int slot) {
    PlaceResult res = r.place(c, id, day, slot);
    std::cout << "  class " << id << " at (day " << day << ", slot " << slot
              << "): " << placeResultText(res) << "\n";
}

int main() {
    ClassList classes;
    classes.insert(1, "CSE2103", "T1", "R1", "E");
    classes.insert(2, "MAT2101", "T2", "R2", "E");
    classes.insert(3, "PHY2101", "T3", "R3", "F");
    classes.insert(4, "CHE2101", "T4", "R4", "E");

    Routine routine;

    std::cout << "Test 1: display an empty routine\n";
    routine.display(classes);

    std::cout << "\nTest 2: place into empty slots (E and F can share a time)\n";
    tryPlace(routine, classes, 1, 0, 0);
    tryPlace(routine, classes, 2, 0, 1);
    tryPlace(routine, classes, 3, 0, 0);

    std::cout << "\nTest 3: slot already used by the same section\n";
    tryPlace(routine, classes, 4, 0, 0);

    std::cout << "\nTest 4: place the same class twice\n";
    tryPlace(routine, classes, 1, 1, 2);

    std::cout << "\nTest 5: class that does not exist\n";
    tryPlace(routine, classes, 99, 0, 2);

    std::cout << "\nTest 6: positions out of range\n";
    tryPlace(routine, classes, 4, 7, 0);
    tryPlace(routine, classes, 4, 0, -1);
    tryPlace(routine, classes, 4, 0, 6);

    std::cout << "\nTest 7: place class 4 in a free slot\n";
    tryPlace(routine, classes, 4, 1, 0);

    std::cout << "\nTest 8: more sections than the table can hold (max "
              << MAX_SECTIONS << ")\n";
    ClassList many;
    Routine big;
    for (int i = 0; i < MAX_SECTIONS + 1; i++) {
        std::string sec(1, static_cast<char>('A' + i));
        many.insert(i + 1, "X" + std::to_string(i), "T", "R", sec);
        tryPlace(big, many, i + 1, 0, 0);
    }

    std::cout << "\nTest 9: display the routine\n";
    routine.display(classes);

    return 0;
}
