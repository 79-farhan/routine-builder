#include "Scheduler.h"

const char* addResultText(AddResult r) {
    switch (r) {
        case ADD_OK:        return "added";
        case ADD_DUPLICATE: return "rejected: duplicate class";
        case ADD_FULL:      return "rejected: class limit reached";
    }
    return "unknown result";
}

// maxClasses is declared first so the queue can be built from it.
Scheduler::Scheduler(int maxClassCount)
    : maxClasses(maxClassCount < 1 ? 1 : maxClassCount),
      waiting(maxClasses) {}

// O(n): the duplicate check inside the list insert walks the list.
// The queue enqueue is O(1) and cannot fail, because the queue never holds
// more ids than the list holds classes and the list is capped at maxClasses.
AddResult Scheduler::addClass(int id, const std::string& course, const std::string& teacher,
                              const std::string& room, const std::string& section) {
    if (classes.size() >= maxClasses) {
        return ADD_FULL;
    }
    if (!classes.insert(id, course, teacher, room, section)) {
        return ADD_DUPLICATE;
    }
    if (!waiting.enqueue(id)) {   // safety net, should never happen
        classes.remove(id);
        return ADD_FULL;
    }
    return ADD_OK;
}

// O(S * n): the routine's place check dominates. The class leaves the queue
// only after it was placed, so a rejected class keeps its spot at the front.
bool Scheduler::placeNext(int day, int slot, int& classId, PlaceResult& result) {
    if (!waiting.peek(classId)) {
        return false;
    }
    result = routine.place(classes, classId, day, slot);
    if (result == PLACE_OK) {
        int removed = -1;
        waiting.dequeue(removed);
        undo.push(makePlaceAction(classId, day, slot));
    }
    return true;
}

// O(1): one dequeue and one enqueue. The enqueue cannot fail because the
// dequeue just freed a place.
bool Scheduler::skipNext(int& classId) {
    if (!waiting.dequeue(classId)) {
        return false;
    }
    waiting.enqueue(classId);
    return true;
}

// O(S * n): same cost as the routine move. A MOVE action is pushed only when
// the move really happened, so a rejected move leaves the stack untouched.
PlaceResult Scheduler::moveClass(int classId, int newDay, int newSlot,
                                 int& oldDay, int& oldSlot) {
    PlaceResult res = routine.move(classes, classId, newDay, newSlot, oldDay, oldSlot);
    if (res == PLACE_OK) {
        undo.push(makeMoveAction(classId, oldDay, oldSlot, newDay, newSlot));
    }
    return res;
}

// O(S * n): dominated by the routine move or unplace. Undo works because the
// stack is LIFO: when an action is popped, the routine is in the exact state
// it was in right after that action, so reversing it cannot clash.
UndoResult Scheduler::undoLast(UndoAction& undone) {
    if (!undo.pop(undone)) {
        return UNDO_NOTHING;
    }

    if (undone.type == ACTION_PLACE) {
        int day = -1;
        int slot = -1;
        if (!routine.unplace(undone.classId, day, slot)) {
            return UNDO_BROKEN;
        }
        if (!waiting.pushFront(undone.classId)) {
            return UNDO_BROKEN;
        }
    } else {
        int oldDay = -1;
        int oldSlot = -1;
        PlaceResult res = routine.move(classes, undone.classId,
                                       undone.fromDay, undone.fromSlot, oldDay, oldSlot);
        if (res != PLACE_OK) {
            return UNDO_BROKEN;
        }
    }
    return UNDO_OK;
}

const ClassList& Scheduler::getClasses() const { return classes; }
const ClassQueue& Scheduler::getQueue() const { return waiting; }
const Routine& Scheduler::getRoutine() const { return routine; }
const UndoStack& Scheduler::getUndo() const { return undo; }

const char* deleteResultText(DeleteResult r) {
    switch (r) {
        case DELETE_OK:        return "deleted";
        case DELETE_NOT_FOUND: return "rejected: class not found";
        case DELETE_PLACED:    return "rejected: class is placed, undo it first";
        case DELETE_BROKEN:    return "internal error";
    }
    return "unknown result";
}

const char* updateResultText(UpdateResult r) {
    switch (r) {
        case UPDATE_OK:        return "updated";
        case UPDATE_NOT_FOUND: return "rejected: class not found";
        case UPDATE_PLACED:    return "rejected: class is placed, undo it first";
        case UPDATE_DUPLICATE: return "rejected: duplicate class";
    }
    return "unknown result";
}

// O(n): list search, queue removal and list removal are each at most one pass.
// A placed class is refused: its id sits on the undo stack, and removing it
// would make that undo fail. Nothing changes unless every check passes.
DeleteResult Scheduler::deleteClass(int id) {
    if (classes.search(id) == nullptr) {
        return DELETE_NOT_FOUND;
    }
    if (routine.isPlaced(id)) {
        return DELETE_PLACED;
    }
    if (!waiting.removeValue(id)) {   // safety net, should never happen
        return DELETE_BROKEN;
    }
    classes.remove(id);
    return DELETE_OK;
}

// O(n): list search plus the list's own duplicate check. A placed class is
// refused: a new teacher or room could create a clash that nothing rechecks.
UpdateResult Scheduler::updateClass(int id, const std::string& course,
                                    const std::string& teacher, const std::string& room,
                                    const std::string& section) {
    if (classes.search(id) == nullptr) {
        return UPDATE_NOT_FOUND;
    }
    if (routine.isPlaced(id)) {
        return UPDATE_PLACED;
    }
    if (!classes.update(id, course, teacher, room, section)) {
        return UPDATE_DUPLICATE;
    }
    return UPDATE_OK;
}
