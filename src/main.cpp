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
    classes.insert(2, "MAT2101", "T2", "R2", "F");
    classes.insert(3, "PHY2101", "T1", "R3", "F");
    classes.insert(4, "CHE2101", "T4", "R1", "G");
    classes.insert(5, "ENG2101", "T5", "R5", "G");
    classes.insert(6, "BUS2101", "T1", "R1", "H");

    Routine routine;

    std::cout << "Test 1: place the first class\n";
    tryPlace(routine, classes, 1, 0, 0);

    std::cout << "\nTest 2: same teacher in another section at the same time\n";
    tryPlace(routine, classes, 3, 0, 0);

    std::cout << "\nTest 3: same teacher at a different time\n";
    tryPlace(routine, classes, 3, 0, 1);

    std::cout << "\nTest 4: same room used by another section at the same time\n";
    tryPlace(routine, classes, 4, 0, 0);

    std::cout << "\nTest 5: same room at a different time\n";
    tryPlace(routine, classes, 4, 0, 2);

    std::cout << "\nTest 6: teacher and room both clash (teacher is reported first)\n";
    tryPlace(routine, classes, 6, 0, 0);

    std::cout << "\nTest 7: different teacher and room in another section, same time\n";
    tryPlace(routine, classes, 2, 0, 0);

    std::cout << "\nTest 8: section busy is reported before any teacher or room clash\n";
    tryPlace(routine, classes, 5, 0, 2);

    std::cout << "\nTest 9: rejected classes must not leave empty sections behind\n";
    routine.display(classes);

    std::cout << "\nTest 10: more sections than the table can hold (max "
              << MAX_SECTIONS << ")\n";
    ClassList many;
    Routine big;
    for (int i = 0; i < MAX_SECTIONS + 1; i++) {
        std::string sec(1, static_cast<char>('A' + i));
        many.insert(i + 1, "X" + std::to_string(i), "T" + std::to_string(i),
                    "R" + std::to_string(i), sec);
        tryPlace(big, many, i + 1, 0, 0);
    }

    return 0;
}
