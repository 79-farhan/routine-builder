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
        case PLACE_NOT_PLACED:       return "rejected: class is not placed yet";
        case PLACE_SAME_SLOT:        return "rejected: class is already in that slot";
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
// The out parameters are only written when the class is found.
bool Routine::locate(int classId, int& outSection, int& outDay, int& outSlot) const {
    for (int s = 0; s < sectionCount; s++) {
        for (int d = 0; d < DAYS; d++) {
            for (int t = 0; t < SLOTS; t++) {
                if (grid[s][d][t] == classId) {
                    outSection = s;
                    outDay = d;
                    outSlot = t;
                    return true;
                }
            }
        }
    }
    return false;
}

// Same cost as locate.
bool Routine::isPlaced(int classId) const {
    int s = -1;
    int d = -1;
    int t = -1;
    return locate(classId, s, d, t);
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

// O(S * DAYS * SLOTS): one locate, then one write. Returns false when the
// class is not placed. On success day and slot hold the position it left,
// which is what the undo stack needs.
bool Routine::unplace(int classId, int& day, int& slot) {
    int s = -1;
    if (!locate(classId, s, day, slot)) {
        return false;
    }
    grid[s][day][slot] = EMPTY_SLOT;
    return true;
}

// O(S * n): same cost as place. Nothing changes unless every check passes.
// The old cell is cleared only at the very end. oldDay and oldSlot hold the
// class's current position whenever it is placed, even if the move is rejected.
PlaceResult Routine::move(const ClassList& classes, int classId, int newDay, int newSlot,
                          int& oldDay, int& oldSlot) {
    if (newDay < 0 || newDay >= DAYS || newSlot < 0 || newSlot >= SLOTS) {
        return PLACE_BAD_POSITION;
    }

    ClassNode* cls = classes.search(classId);
    if (cls == nullptr) {
        return PLACE_CLASS_NOT_FOUND;
    }

    int s = -1;
    if (!locate(classId, s, oldDay, oldSlot)) {
        return PLACE_NOT_PLACED;
    }

    if (oldDay == newDay && oldSlot == newSlot) {
        return PLACE_SAME_SLOT;
    }

    if (grid[s][newDay][newSlot] != EMPTY_SLOT) {
        return PLACE_SECTION_BUSY;
    }

    PlaceResult clash = checkResourceClash(classes, cls, newDay, newSlot);
    if (clash != PLACE_OK) {
        return clash;
    }

    grid[s][oldDay][oldSlot] = EMPTY_SLOT;
    grid[s][newDay][newSlot] = classId;
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

static const char* const DAY_LABELS[DAYS] = {"Sun", "Mon", "Tue", "Wed", "Thu"};

// O(SLOTS): one pass to find the first and last filled slot, one pass between
// them to count the empty ones. A day with no class or one class has no gap.
int Routine::gapsInDay(int sectionIndex, int day) const {
    int first = -1;
    int last = -1;
    for (int t = 0; t < SLOTS; t++) {
        if (grid[sectionIndex][day][t] != EMPTY_SLOT) {
            if (first == -1) {
                first = t;
            }
            last = t;
        }
    }
    if (first == -1 || first == last) {
        return 0;
    }
    int gaps = 0;
    for (int t = first + 1; t < last; t++) {
        if (grid[sectionIndex][day][t] == EMPTY_SLOT) {
            gaps++;
        }
    }
    return gaps;
}

// O(S + SLOTS). Returns -1 for an unknown section or a bad day.
int Routine::gapsForSectionDay(const std::string& section, int day) const {
    if (day < 0 || day >= DAYS) {
        return -1;
    }
    int s = findSection(section);
    if (s == -1) {
        return -1;
    }
    return gapsInDay(s, day);
}

// O(S + DAYS * SLOTS). Returns -1 for an unknown section.
int Routine::gapsForSection(const std::string& section) const {
    int s = findSection(section);
    if (s == -1) {
        return -1;
    }
    int total = 0;
    for (int d = 0; d < DAYS; d++) {
        total += gapsInDay(s, d);
    }
    return total;
}

// O(S * DAYS * SLOTS): all sizes are small fixed constants.
int Routine::totalGaps() const {
    int total = 0;
    for (int s = 0; s < sectionCount; s++) {
        for (int d = 0; d < DAYS; d++) {
            total += gapsInDay(s, d);
        }
    }
    return total;
}

// O(S * DAYS * SLOTS). Prints gaps per day, per section and overall.
void Routine::printGapReport() const {
    if (sectionCount == 0) {
        std::cout << "No classes placed, no gaps to report.\n";
        return;
    }

    std::cout << "Gap report (empty slots between a section's first and last class of each day)\n";
    for (int s = 0; s < sectionCount; s++) {
        std::cout << "Section " << sectionNames[s] << ":";
        int weekTotal = 0;
        for (int d = 0; d < DAYS; d++) {
            int g = gapsInDay(s, d);
            weekTotal += g;
            std::cout << (d == 0 ? " " : ", ") << DAY_LABELS[d] << " " << g;
        }
        std::cout << " | week total " << weekTotal << "\n";
    }
    std::cout << "All sections: " << totalGaps() << " gap slots\n";
}
