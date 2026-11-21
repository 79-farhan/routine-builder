#include <iostream>
#include "ClassList.h"

int main() {
    ClassList list;

    list.insert(1, "C1", "T1", "R1", "E");
    list.insert(2, "C2", "T2", "R2", "E");
    list.insert(3, "C3", "T3", "R3", "E");
    list.insert(4, "C4", "T4", "R4", "E");
    list.insert(5, "C5", "T5", "R5", "E");

    std::cout << "Test 1: remove a middle node (id 3)\n";
    list.remove(3);
    list.display();
    std::cout << "  size " << list.size() << "\n";

    std::cout << "\nTest 2: remove the head (id 1)\n";
    list.remove(1);
    list.display();
    std::cout << "  size " << list.size() << "\n";

    std::cout << "\nTest 3: remove the tail (id 5)\n";
    list.remove(5);
    list.display();
    std::cout << "  size " << list.size() << "\n";

    std::cout << "\nTest 4: remove a missing id (99)\n";
    if (!list.remove(99)) {
        std::cout << "  not found (correct)\n";
    }

    std::cout << "\nTest 5: insert after tail removal (tail must be correct)\n";
    list.insert(6, "C6", "T6", "R6", "E");
    list.display();
    std::cout << "  size " << list.size() << "\n";

    std::cout << "\nTest 6: remove everything\n";
    list.remove(2);
    list.remove(4);
    list.remove(6);
    list.display();
    std::cout << "  size " << list.size() << "\n";

    std::cout << "\nTest 7: remove from an empty list\n";
    if (!list.remove(1)) {
        std::cout << "  not found (correct)\n";
    }

    std::cout << "\nTest 8: insert after the list was emptied\n";
    list.insert(7, "C7", "T7", "R7", "E");
    list.display();
    std::cout << "  size " << list.size() << "\n";

    return 0;
}
