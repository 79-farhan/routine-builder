#include "ClassList.h"
#include <iostream>

ClassList::ClassList() : head(nullptr), tail(nullptr), count(0) {}

ClassList::~ClassList() {
    ClassNode* cur = head;
    while (cur != nullptr) {
        ClassNode* nextNode = cur->next;  // save before deleting
        delete cur;
        cur = nextNode;
    }
}

// O(1): the tail pointer means we never walk the list to add at the end
bool ClassList::insert(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section) {
    ClassNode* node = new ClassNode;
    node->id = id;
    node->course = course;
    node->teacher = teacher;
    node->room = room;
    node->section = section;
    node->next = nullptr;

    if (head == nullptr) {      // empty list: node is both head and tail
        head = node;
        tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count++;
    return true;
}

// O(n): visits every node once
void ClassList::display() const {
    if (head == nullptr) {
        std::cout << "No classes added yet.\n";
        return;
    }
    ClassNode* cur = head;
    while (cur != nullptr) {
        std::cout << cur->id << " | " << cur->course << " | " << cur->teacher
                  << " | " << cur->room << " | " << cur->section << "\n";
        cur = cur->next;
    }
}

int ClassList::size() const {
    return count;
}
