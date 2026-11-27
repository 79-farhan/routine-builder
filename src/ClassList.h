#ifndef CLASSLIST_H
#define CLASSLIST_H

#include <string>

struct ClassNode {
    int id;
    std::string course;
    std::string teacher;
    std::string room;
    std::string section;
    int session;        // 1 = first weekly meeting of the course, 2 = second, ...
    ClassNode* next;
};

class ClassList {
private:
    ClassNode* head;
    ClassNode* tail;
    int count;

    bool sessionTaken(const ClassNode* skip, const std::string& course,
                      const std::string& section, int session) const;

public:
    ClassList();
    ~ClassList();

    // copying a list would make two lists share nodes and double-delete them
    ClassList(const ClassList&) = delete;
    ClassList& operator=(const ClassList&) = delete;

    // The version without a session means session 1.
    bool insert(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section);
    bool insert(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section, int session);
    bool remove(int id);
    // The version without a session keeps the class's current session.
    bool update(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section);
    bool update(int id, const std::string& course, const std::string& teacher,
                const std::string& room, const std::string& section, int session);
    ClassNode* search(int id) const;
    void display() const;
    int size() const;
};

#endif
