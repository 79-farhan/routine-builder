#include <iostream>
#include "Routine.h"

void setup(Routine& r, const ClassList& c, int id, int day, int slot) {
    PlaceResult res = r.place(c, id, day, slot);
    if (res != PLACE_OK) {
        std::cout << "  setup failed for class " << id << ": "
                  << placeResultText(res) << "\n";
    }
}

int main() {
    ClassList classes;
    classes.insert(1, "C1", "T1", "R1", "E");
    classes.insert(2, "C2", "T2", "R2", "E");
    classes.insert(3, "C3", "T3", "R3", "E");
    classes.insert(4, "C4", "T4", "R4", "E");
    classes.insert(5, "C5", "T5", "R5", "F");
    classes.insert(6, "C6", "T6", "R6", "F");
    classes.insert(7, "C7", "T7", "R7", "F");
    classes.insert(8, "C8", "T8", "R8", "E");

    Routine routine;

    std::cout << "Test 1: gap report on an empty routine\n";
    routine.printGapReport();

    std::cout << "\nTest 2: a single class in a day has no gap\n";
    setup(routine, classes, 1, 0, 0);
    std::cout << "  section E, day 0: " << routine.gapsForSectionDay("E", 0) << " gap slots\n";

    std::cout << "\nTest 3: two classes side by side have no gap\n";
    setup(routine, classes, 2, 0, 1);
    std::cout << "  section E, day 0: " << routine.gapsForSectionDay("E", 0) << " gap slots\n";

    std::cout << "\nTest 4: classes in slots 1, 2 and 5 leave two empty slots between them\n";
    setup(routine, classes, 3, 0, 4);
    std::cout << "  section E, day 0: " << routine.gapsForSectionDay("E", 0) << " gap slots\n";

    std::cout << "\nTest 5: unknown section and day out of range\n";
    std::cout << "  unknown section: " << routine.gapsForSectionDay("Z", 0) << "\n";
    std::cout << "  day out of range: " << routine.gapsForSectionDay("E", 9) << "\n";
    std::cout << "  weekly total, unknown section: " << routine.gapsForSection("Z") << "\n";

    std::cout << "\nTest 6: full report\n";
    setup(routine, classes, 4, 1, 1);
    setup(routine, classes, 8, 1, 5);
    setup(routine, classes, 5, 0, 2);
    setup(routine, classes, 6, 0, 3);
    setup(routine, classes, 7, 0, 5);
    routine.printGapReport();

    std::cout << "\nTest 7: moving class 3 from slot 5 to slot 3 closes the gap\n";
    int oldDay = -1;
    int oldSlot = -1;
    PlaceResult r = routine.move(classes, 3, 0, 2, oldDay, oldSlot);
    std::cout << "  class 3 to (day 0, slot 2): "
              << (r == PLACE_OK ? "moved" : placeResultText(r)) << "\n";
    routine.printGapReport();

    std::cout << "\nTest 8: removing the last class of a day shrinks its span\n";
    int day = -1;
    int slot = -1;
    if (routine.unplace(8, day, slot)) {
        std::cout << "  class 8 removed\n";
    }
    routine.printGapReport();

    return 0;
}
