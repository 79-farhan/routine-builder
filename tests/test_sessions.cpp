#include "test_helper.h"
#include "ClassList.h"

int main() {
    section("Same course twice a week");
    ClassList list;
    check(list.insert(1, "CSE2101", "T1", "R1", "3E", 1), "insert session 1 of a course");
    check(list.insert(2, "CSE2101", "T1", "R2", "3E", 2),
          "insert session 2 of the same course in the same section");
    check(list.size() == 2, "both sessions are stored");
    ClassNode* n = list.search(2);
    check(n != nullptr && n->session == 2 && n->room == "R2", "session number is stored");

    section("Duplicates are still rejected");
    check(!list.insert(3, "CSE2101", "T9", "R9", "3E", 1),
          "same course, section and session is rejected");
    check(!list.insert(2, "OTHER", "T9", "R9", "3F", 1), "duplicate id is rejected");
    check(list.size() == 2, "rejected inserts do not change the size");

    section("Other sections and courses are independent");
    check(list.insert(3, "CSE2101", "T1", "R3", "3F", 1),
          "same course and session in another section is accepted");
    check(list.insert(4, "CSE2103", "T2", "R4", "3E", 1),
          "another course in the same section is accepted");

    section("Invalid session");
    check(!list.insert(5, "CSE2105", "T3", "R5", "3E", 0), "session 0 is rejected");
    check(!list.insert(5, "CSE2105", "T3", "R5", "3E", -2), "negative session is rejected");
    check(list.size() == 4, "invalid sessions do not change the size");

    section("Insert without a session means session 1");
    check(list.insert(5, "CSE2105", "T3", "R5", "3E"), "insert without a session");
    n = list.search(5);
    check(n != nullptr && n->session == 1, "it is stored as session 1");
    check(!list.insert(6, "CSE2105", "T3", "R6", "3E"),
          "inserting it again is rejected as a duplicate");

    section("Update with a session");
    check(!list.update(2, "CSE2101", "T1", "R2", "3E", 1),
          "update that moves class 2 onto session 1 is rejected");
    n = list.search(2);
    check(n != nullptr && n->session == 2, "rejected update leaves the session unchanged");
    check(list.update(2, "CSE2101", "T1", "R2", "3E", 3), "update to a free session number");
    n = list.search(2);
    check(n != nullptr && n->session == 3, "new session number is stored");
    check(list.update(2, "CSE2101", "T7", "R7", "3E", 3),
          "update keeping its own session is accepted");
    check(!list.update(99, "C", "T", "R", "S", 1), "update of a missing id returns false");

    section("Update without a session keeps it");
    check(list.update(2, "CSE2101", "T8", "R8", "3E"), "update without a session is accepted");
    n = list.search(2);
    check(n != nullptr && n->session == 3 && n->teacher == "T8",
          "session is kept and the teacher changed");
    check(!list.update(99, "C", "T", "R", "S"), "update without a session of a missing id fails");

    return finishTests();
}
