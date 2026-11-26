#include "Routine.h"
#include <iostream>
#include <iomanip>

const char* placeResultText(PlaceResult r) {
    switch (r) {
        case PLACE_OK:               return "placed";
        case PLACE_BAD_POSITION:     return "rejected: day or slot out of range";
        case PLACE_CLASS_NOT_FOUND:  return "rejected: class not found";
        case PLACE_ALREADY_PLACED:   return "rejected: class is already placed";
        case PLACE_SECTION_BUSY:     return "rejected: section already has a class in this slot";
        case PLACE_TEACHER_CLASH:    return "rejected: teacher is busy in another section at this time";
        case PLACE_ROOM_CLASH:       return "rejected: room is taken by another section at this time";
        case PLACE_NO_SECTION_SPACE: return "rejected: no room for another section";
    }
    return "unknown result";
}

Routine::Routine() : sectionCount(0) {
    for (int s = 0; s < MAX_SECTIONS; s++) {
        for (int d = 0; d < DAYS; d++) {
            for (int t = 0; t < SLOTS; t++) {
                grid[s][d][t] = EMPTY_SLOT;
            }
        }
    }
}

// O(S): S is the number of sections, at most MAX_SECTIONS. Returns -1 if unknown.
int Routine::findSection(const std::string& name) const {
    for (int s = 0; s < sectionCount; s++) {
        if (sectionNames[s] == name) {
            return s;
        }
    }
    return -1;
}

// O(1). Returns -1 when the table already holds MAX_SECTIONS sections.
int Routine::addSection(const std::string& name) {
    if (sectionCount >= MAX_SECTIONS) {
        return -1;
    }
    sectionNames[sectionCount] = name;
    sectionCount++;
    return sectionCount - 1;
}

// O(S * DAYS * SLOTS): scans every cell. All three sizes are small fixed
// constants, so this is a constant amount of work in practice.
bool Routine::isPlaced(int classId) const {
    for (int s = 0; s < sectionCount; s++) {
        for (int d = 0; d < DAYS; d++) {
            for (int t = 0; t < SLOTS; t++) {
                if (grid[s][d][t] == classId) {
                    return true;
                }
            }
        }
    }
    return false;
}

// O(S * n): looks at the same (day, slot) cell in every section, and each
// filled cell costs one list lookup. A teacher clash is returned straight
// away, so it always wins over a room clash.
PlaceResult Routine::checkResourceClash(const ClassList& classes, const ClassNode* cls,
                                        int day, int slot) const {
    bool roomClash = false;
    for (int s = 0; s < sectionCount; s++) {
        int otherId = grid[s][day][slot];
        if (otherId == EMPTY_SLOT) {
            continue;
        }
        ClassNode* other = classes.search(otherId);
        if (other == nullptr) {
            continue;
        }
        if (other->teacher == cls->teacher) {
            return PLACE_TEACHER_CLASH;
        }
        if (other->room == cls->room) {
            roomClash = true;
        }
    }
    return roomClash ? PLACE_ROOM_CLASH : PLACE_OK;
}

// O(n) for the class lookup, plus O(S * n) for the clash check, so O(S * n)
// overall, and S is at most 6. Checks run cheapest first, and nothing is
// written (not even a new section) until every check has passed.
PlaceResult Routine::place(const ClassList& classes, int classId, int day, int slot) {
    if (day < 0 || day >= DAYS || slot < 0 || slot >= SLOTS) {
        return PLACE_BAD_POSITION;
    }

    ClassNode* cls = classes.search(classId);
    if (cls == nullptr) {
        return PLACE_CLASS_NOT_FOUND;
    }

    if (isPlaced(classId)) {
        return PLACE_ALREADY_PLACED;
    }

    int s = findSection(cls->section);
    if (s == -1 && sectionCount >= MAX_SECTIONS) {
        return PLACE_NO_SECTION_SPACE;
    }

    if (s != -1 && grid[s][day][slot] != EMPTY_SLOT) {
        return PLACE_SECTION_BUSY;
    }

    PlaceResult clash = checkResourceClash(classes, cls, day, slot);
    if (clash != PLACE_OK) {
        return clash;
    }

    if (s == -1) {
        s = addSection(cls->section);
    }
    grid[s][day][slot] = classId;
    return PLACE_OK;
}

// O(S * DAYS * SLOTS * n): every filled cell triggers a list lookup.
void Routine::display(const ClassList& classes) const {
    if (sectionCount == 0) {
        std::cout << "Routine is empty. No section has a placed class.\n";
        return;
    }

    const char* dayNames[DAYS] = {"Sun", "Mon", "Tue", "Wed", "Thu"};

    for (int s = 0; s < sectionCount; s++) {
        std::cout << "Section " << sectionNames[s] << "\n";

        std::cout << std::left << std::setw(6) << "";
        for (int t = 0; t < SLOTS; t++) {
            std::cout << std::setw(9) << ("Slot " + std::to_string(t + 1));
        }
        std::cout << "\n";

        for (int d = 0; d < DAYS; d++) {
            std::cout << std::setw(6) << dayNames[d];
            for (int t = 0; t < SLOTS; t++) {
                std::string cell = "--";
                int id = grid[s][d][t];
                if (id != EMPTY_SLOT) {
                    ClassNode* c = classes.search(id);
                    cell = (c != nullptr) ? c->course : "?";
                }
                std::cout << std::setw(9) << cell;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
}
