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
    PLACE_NO_SECTION_SPACE
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
    PlaceResult checkResourceClash(const ClassList& classes, const ClassNode* cls,
                                   int day, int slot) const;

public:
    Routine();

    PlaceResult place(const ClassList& classes, int classId, int day, int slot);
    bool isPlaced(int classId) const;
    void display(const ClassList& classes) const;
};

#endif
