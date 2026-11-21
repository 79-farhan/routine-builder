#include <iostream>
#include "ClassList.h"

int main() {
    ClassList list;

    list.insert(1, "C1", "T1", "R1", "E");
    list.insert(2, "C2", "T2", "R2", "E");
    list.insert(3, "C1", "T3", "R3", "F");

    std::cout << "Test 1: update teacher and room of id 2\n";
    list.update(2, "C2", "NewT", "NewR", "E");
    list.display();

    std::cout << "\nTest 2: update a missing id (99)\n";
    if (!list.update(99, "C1", "T", "R", "E")) {
        std::cout << "  not found (correct)\n";
    }

    std::cout << "\nTest 3: update id 3 so it duplicates course+section of id 1\n";
    if (!list.update(3, "C1", "T9", "R9", "E")) {
        std::cout << "  rejected (correct), list unchanged:\n";
    }
    list.display();

    std::cout << "\nTest 4: update id 3, keeping its own course and section\n";
    if (list.update(3, "C1", "T9", "R9", "F")) {
        std::cout << "  accepted (correct):\n";
    }
    list.display();

    std::cout << "\nTest 5: change course and section of id 3\n";
    list.update(3, "C9", "T9", "R9", "G");
    list.display();

    std::cout << "\nTest 6: update on an empty list\n";
    ClassList empty;
    if (!empty.update(1, "C1", "T1", "R1", "E")) {
        std::cout << "  not found (correct)\n";
    }

    return 0;
}
