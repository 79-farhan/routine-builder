#ifndef CLASSLIST_H
#define CLASSLIST_H

#include <string>

struct ClassNode {
    int id;
    std::string course;
    std::string teacher;
    std::string room;
    std::string section;
    ClassNode* next;
};

class ClassList {
private:
    ClassNode* head;
    ClassNode* tail;
    int count;

    bool isDuplicate(int id, const std::string& course, const std::string& section) const;
    bool courseSectionTaken(int skipId, const std::string& course,
                            const std::string& section) const;

public:
    ClassList();
    ~ClassList();

    // copying a list would make two lists share nodes and double-delete them
    ClassList(const ClassList&) = delete;
    ClassList& operator=(const ClassList&) = delete;

    bool insert(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section);
    bool remove(int id);
    bool update(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section);
    ClassNode* search(int id) const;
    void display() const;
    int size() const;
};

#endif
