#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include "ClassList.h"
#include "ClassQueue.h"
#include "Routine.h"
#include "UndoStack.h"

enum AddResult { ADD_OK, ADD_DUPLICATE, ADD_FULL };
enum UndoResult { UNDO_OK, UNDO_NOTHING, UNDO_BROKEN };
enum DeleteResult { DELETE_OK, DELETE_NOT_FOUND, DELETE_PLACED, DELETE_BROKEN };
enum UpdateResult { UPDATE_OK, UPDATE_NOT_FOUND, UPDATE_PLACED, UPDATE_DUPLICATE };

const char* addResultText(AddResult r);
const char* deleteResultText(DeleteResult r);
const char* updateResultText(UpdateResult r);

// Owns the four structures and keeps them consistent:
//   every class is in the list, waiting classes are also in the queue,
//   placed classes are in the routine, and every change is on the undo stack.
class Scheduler {
private:
    int maxClasses;
    ClassList classes;
    ClassQueue waiting;
    Routine routine;
    UndoStack undo;

public:
    explicit Scheduler(int maxClassCount);

    AddResult addClass(int id, const std::string& course, const std::string& teacher,
                       const std::string& room, const std::string& section);

    // Only classes that are not placed can be deleted or updated.
    DeleteResult deleteClass(int id);
    UpdateResult updateClass(int id, const std::string& course, const std::string& teacher,
                             const std::string& room, const std::string& section);

    // Tries to place the class at the front of the queue. Returns false when
    // the queue is empty. Otherwise classId is the class that was tried and
    // result says what happened.
    bool placeNext(int day, int slot, int& classId, PlaceResult& result);

    // Sends the front class to the back of the queue. False when empty.
    bool skipNext(int& classId);

    PlaceResult moveClass(int classId, int newDay, int newSlot, int& oldDay, int& oldSlot);
    UndoResult undoLast(UndoAction& undone);

    const ClassList& getClasses() const;
    const ClassQueue& getQueue() const;
    const Routine& getRoutine() const;
    const UndoStack& getUndo() const;
};

#endif
