#include <iostream>
#include "ClassList.h"

int main() {
    ClassList list;

    std::cout << "Empty list test:\n";
    list.display();

    list.insert(1, "CSE2103", "Mr. A", "R101", "E");
    list.insert(2, "MAT2101", "Ms. B", "R102", "E");
    list.insert(3, "PHY2101", "Mr. C", "R101", "F");

    std::cout << "\nAfter 3 inserts (size " << list.size() << "):\n";
    list.display();
    return 0;
}
