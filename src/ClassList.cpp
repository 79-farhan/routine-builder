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

// O(n): true if some class other than skip already has this course, section
// and session. skip may be nullptr when nothing should be ignored.
bool ClassList::sessionTaken(const ClassNode* skip, const std::string& course,
                             const std::string& section, int session) const {
    ClassNode* cur = head;
    while (cur != nullptr) {
        if (cur != skip && cur->course == course &&
            cur->section == section && cur->session == session) {
            return true;
        }
        cur = cur->next;
    }
    return false;
}

// Session 1 by default.
bool ClassList::insert(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section) {
    return insert(id, course, teacher, room, section, 1);
}

// O(n) for the id search and the session check, then O(1) to attach at the
// tail. Returns false and changes nothing if the session is below 1, the id
// exists, or the course/section/session combination exists. Every check runs
// before new is called, so a rejected class never leaks a node.
bool ClassList::insert(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section, int session) {
    if (session < 1) {
        return false;
    }
    if (search(id) != nullptr || sessionTaken(nullptr, course, section, session)) {
        return false;
    }

    ClassNode* node = new ClassNode;
    node->id = id;
    node->course = course;
    node->teacher = teacher;
    node->room = room;
    node->section = section;
    node->session = session;
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

// Keeps the class's current session.
bool ClassList::update(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section) {
    ClassNode* node = search(id);
    if (node == nullptr) {
        return false;
    }
    return update(id, course, teacher, room, section, node->session);
}

// O(n): search is O(n), the session check is O(n), the field changes are O(1).
// Returns false if the id is missing, the session is below 1, or the new
// course/section/session would duplicate another class. On failure the node
// is left exactly as it was.
bool ClassList::update(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section, int session) {
    if (session < 1) {
        return false;
    }
    ClassNode* node = search(id);
    if (node == nullptr) {
        return false;
    }
    if (sessionTaken(node, course, section, session)) {
        return false;
    }
    node->course = course;
    node->teacher = teacher;
    node->room = room;
    node->section = section;
    node->session = session;
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
                  << " | " << cur->room << " | " << cur->section
                  << " | session " << cur->session << "\n";
        cur = cur->next;
    }
}

int ClassList::size() const {
    return count;
}
