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

// O(n): walks the list once. Duplicate means same id,
// or same course already added for the same section.
bool ClassList::isDuplicate(int id, const std::string& course,
                            const std::string& section) const {
    ClassNode* cur = head;
    while (cur != nullptr) {
        if (cur->id == id) {
            return true;
        }
        if (cur->course == course && cur->section == section) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

// O(n) because of the duplicate check, then O(1) to attach at the tail.
// Returns false and changes nothing if the class is a duplicate.
bool ClassList::insert(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section) {
    if (isDuplicate(id, course, section)) {
        return false;
    }

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

// O(n): in the worst case the class is last or missing.
// Returns nullptr when not found (also true for an empty list).
ClassNode* ClassList::search(int id) const {
    ClassNode* cur = head;
    while (cur != nullptr) {
        if (cur->id == id) {
            return cur;
        }
        cur = cur->next;
    }
    return nullptr;
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

// O(n): must walk to find the node. Unlinking and deleting is O(1).
// Returns false if no class has this id (including an empty list).
bool ClassList::remove(int id) {
    ClassNode* prev = nullptr;
    ClassNode* cur = head;

    while (cur != nullptr && cur->id != id) {
        prev = cur;
        cur = cur->next;
    }

    if (cur == nullptr) {        // not found
        return false;
    }

    if (prev == nullptr) {       // removing the head
        head = cur->next;
    } else {                     // removing a middle or tail node
        prev->next = cur->next;
    }

    if (cur == tail) {           // removed the last node: tail steps back
        tail = prev;
    }

    delete cur;
    count--;
    return true;
}
