#ifndef ROUTINE_H
#define ROUTINE_H

#include <string>
#include "ClassList.h"

const int DAYS = 5;
const int SLOTS = 6;
const int MAX_SECTIONS = 6;
const int EMPTY_SLOT = -1;

enum PlaceResult {
    PLACE_OK,
    PLACE_BAD_POSITION,
    PLACE_CLASS_NOT_FOUND,
    PLACE_ALREADY_PLACED,
    PLACE_SECTION_BUSY,
    PLACE_TEACHER_CLASH,
    PLACE_ROOM_CLASH,
    PLACE_NO_SECTION_SPACE,
    PLACE_NOT_PLACED,
    PLACE_SAME_SLOT
};

const char* placeResultText(PlaceResult r);

// The weekly routine: one days x slots grid per section. Each cell holds a
// class id, or EMPTY_SLOT when nothing is placed there.
class Routine {
private:
    int grid[MAX_SECTIONS][DAYS][SLOTS];
    std::string sectionNames[MAX_SECTIONS];
    int sectionCount;

    int findSection(const std::string& name) const;
    int addSection(const std::string& name);
    bool locate(int classId, int& outSection, int& outDay, int& outSlot) const;
    PlaceResult checkResourceClash(const ClassList& classes, const ClassNode* cls,
                                   int day, int slot) const;
    int gapsInDay(int sectionIndex, int day) const;

public:
    Routine();

    PlaceResult place(const ClassList& classes, int classId, int day, int slot);
    bool unplace(int classId, int& day, int& slot);
    PlaceResult move(const ClassList& classes, int classId, int newDay, int newSlot,
                     int& oldDay, int& oldSlot);
    bool isPlaced(int classId) const;
    void display(const ClassList& classes) const;

    // Gap = empty slot between a section's first and last class of a day.
    // The first two return -1 for an unknown section or a day out of range.
    int gapsForSectionDay(const std::string& section, int day) const;
    int gapsForSection(const std::string& section) const;
    int totalGaps() const;
    void printGapReport() const;
};

#endif
