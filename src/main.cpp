#include <iostream>
#include "ClassList.h"

int main() {
    ClassList list;

    std::cout << "Test 1: search on empty list\n";
    if (list.search(1) == nullptr) {
        std::cout << "  not found (correct)\n";
    }

    list.insert(1, "CSE2103", "Mr. A", "R101", "E");
    list.insert(2, "MAT2101", "Ms. B", "R102", "E");
    list.insert(3, "PHY2101", "Mr. C", "R101", "F");

    std::cout << "\nTest 2: list after 3 inserts (size " << list.size() << ")\n";
    list.display();

    std::cout << "\nTest 3: duplicate id\n";
    if (!list.insert(1, "CHE2101", "Mr. D", "R103", "E")) {
        std::cout << "  rejected (correct)\n";
    }

    std::cout << "\nTest 4: same course in same section\n";
    if (!list.insert(9, "CSE2103", "Mr. Z", "R104", "E")) {
        std::cout << "  rejected (correct)\n";
    }

    std::cout << "\nTest 5: same course in a different section\n";
    if (list.insert(4, "CSE2103", "Mr. A", "R101", "F")) {
        std::cout << "  accepted (correct)\n";
    }

    std::cout << "\nTest 6: search existing and missing\n";
    ClassNode* found = list.search(2);
    if (found != nullptr) {
        std::cout << "  id 2 found: " << found->course << "\n";
    }
    if (list.search(99) == nullptr) {
        std::cout << "  id 99 not found (correct)\n";
    }

    std::cout << "\nFinal size: " << list.size() << "\n";
    return 0;
}
