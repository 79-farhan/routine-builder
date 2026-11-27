#include "test_helper.h"
#include "ClassList.h"

int main() {
    section("Empty list");
    ClassList list;
    check(list.size() == 0, "new list has size 0");
    check(list.search(1) == nullptr, "search on an empty list returns null");
    check(!list.remove(1), "remove on an empty list returns false");
    check(!list.update(1, "C", "T", "R", "S"), "update on an empty list returns false");

    section("Insert and search");
    check(list.insert(1, "C1", "T1", "R1", "E"), "insert first class");
    check(list.insert(2, "C2", "T2", "R2", "E"), "insert second class");
    check(list.insert(3, "C3", "T3", "R3", "F"), "insert third class");
    check(list.size() == 3, "size is 3 after three inserts");
    ClassNode* n = list.search(2);
    check(n != nullptr && n->course == "C2" && n->teacher == "T2" &&
          n->room == "R2" && n->section == "E", "search finds class 2 with the right fields");
    check(list.search(99) == nullptr, "search for a missing id returns null");

    section("Duplicates");
    check(!list.insert(1, "C9", "T9", "R9", "G"), "duplicate id is rejected");
    check(!list.insert(9, "C1", "T9", "R9", "E"), "same course in the same section is rejected");
    check(list.insert(4, "C1", "T1", "R1", "F"), "same course in a different section is accepted");
    check(list.size() == 4, "rejected inserts do not change the size");

    section("Remove");
    check(list.remove(2), "remove a middle node");
    check(list.search(2) == nullptr && list.size() == 3, "removed node is gone and size drops");
    check(list.remove(1), "remove the head");
    check(list.search(1) == nullptr && list.search(3) != nullptr, "head is gone, the rest is intact");
    check(list.remove(4), "remove the tail");
    check(list.insert(5, "C5", "T5", "R5", "E"), "insert after removing the tail");
    check(list.search(5) != nullptr && list.size() == 2, "node inserted after tail removal is reachable");
    check(!list.remove(99), "remove a missing id returns false");
    check(list.size() == 2, "failed remove does not change the size");
    check(list.remove(3) && list.remove(5), "remove all remaining nodes");
    check(list.size() == 0 && list.search(5) == nullptr, "list is empty again");
    check(list.insert(6, "C6", "T6", "R6", "E") && list.size() == 1, "insert after the list was emptied");

    section("Update");
    ClassList u;
    u.insert(1, "C1", "T1", "R1", "E");
    u.insert(2, "C2", "T2", "R2", "E");
    u.insert(3, "C1", "T3", "R3", "F");
    check(u.update(2, "C2", "NEWT", "NEWR", "E"), "update teacher and room");
    n = u.search(2);
    check(n != nullptr && n->teacher == "NEWT" && n->room == "NEWR", "updated fields are stored");
    check(!u.update(99, "C1", "T", "R", "E"), "update a missing id returns false");
    check(!u.update(3, "C1", "T9", "R9", "E"), "update that duplicates course and section is rejected");
    n = u.search(3);
    check(n != nullptr && n->teacher == "T3" && n->section == "F", "rejected update leaves the class unchanged");
    check(u.update(3, "C1", "T9", "R9", "F"), "update keeping its own course and section is accepted");
    check(u.update(3, "C9", "T9", "R9", "G"), "update to a new course and section");
    n = u.search(3);
    check(n != nullptr && n->course == "C9" && n->section == "G" && u.size() == 3,
          "update does not change the size");

    return finishTests();
}
