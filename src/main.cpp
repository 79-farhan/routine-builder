#include <iostream>
#include <string>
#include <cctype>
#include "Scheduler.h"

const int MAX_CLASSES = 30;

static const char* const DAY_NAMES[DAYS] = {"Sun", "Mon", "Tue", "Wed", "Thu"};

static std::string trim(const std::string& s) {
    std::size_t a = 0;
    while (a < s.size() && std::isspace(static_cast<unsigned char>(s[a]))) {
        a++;
    }
    std::size_t b = s.size();
    while (b > a && std::isspace(static_cast<unsigned char>(s[b - 1]))) {
        b--;
    }
    return s.substr(a, b - a);
}

static std::string toUpper(std::string s) {
    for (std::size_t i = 0; i < s.size(); i++) {
        s[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(s[i])));
    }
    return s;
}

// Returns false when the input ends (Ctrl+D or a closed pipe).
static bool readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    return static_cast<bool>(std::getline(std::cin, out));
}

// Keeps asking until the person types a whole number (digits only).
static bool readInt(const std::string& prompt, int& value) {
    while (true) {
        std::string line;
        if (!readLine(prompt, line)) {
            return false;
        }
        line = trim(line);
        bool ok = !line.empty() && line.size() <= 6;
        for (std::size_t i = 0; ok && i < line.size(); i++) {
            if (!std::isdigit(static_cast<unsigned char>(line[i]))) {
                ok = false;
            }
        }
        if (!ok) {
            std::cout << "  Please type a whole number (digits only).\n";
            continue;
        }
        value = std::stoi(line);
        return true;
    }
}

// Keeps asking until the text is not empty. The result is trimmed and uppercase.
static bool readText(const std::string& prompt, std::string& out) {
    while (true) {
        std::string line;
        if (!readLine(prompt, line)) {
            return false;
        }
        line = trim(line);
        if (line.empty()) {
            std::cout << "  This cannot be empty.\n";
            continue;
        }
        out = toUpper(line);
        return true;
    }
}

// Reads a 1-based day and slot and converts them to 0-based. A value out of
// range is passed on as it is, and the routine rejects it with its own message.
static bool readPosition(int& day, int& slot) {
    std::string dayPrompt = "  Day (1=" + std::string(DAY_NAMES[0]) + " ... " +
                            std::to_string(DAYS) + "=" + DAY_NAMES[DAYS - 1] + "): ";
    if (!readInt(dayPrompt, day)) {
        return false;
    }
    if (!readInt("  Slot (1-" + std::to_string(SLOTS) + "): ", slot)) {
        return false;
    }
    day = day - 1;
    slot = slot - 1;
    return true;
}

static std::string positionText(int day, int slot) {
    return std::string(DAY_NAMES[day]) + " slot " + std::to_string(slot + 1);
}

static void printMenu() {
    std::cout << "\n=== Smart Class Routine Builder ===\n"
              << " 1. Add a class\n"
              << " 2. Show waiting queue\n"
              << " 3. Place next class from the queue\n"
              << " 4. Skip next class (send to back of queue)\n"
              << " 5. Move a placed class\n"
              << " 6. Undo last action\n"
              << " 7. Delete a waiting class\n"
              << " 8. Update a waiting class\n"
              << " 9. Show routine\n"
              << "10. Show gap report\n"
              << "11. Show all classes\n"
              << "12. Show undo history\n"
              << " 0. Exit\n";
}

// Each handler returns false only when the input has ended.

static bool handleAdd(Scheduler& s, int& nextId) {
    std::string course;
    std::string teacher;
    std::string room;
    std::string section;
    if (!readText("  Course: ", course)) return false;
    if (!readText("  Teacher: ", teacher)) return false;
    if (!readText("  Room: ", room)) return false;
    if (!readText("  Section: ", section)) return false;

    AddResult r = s.addClass(nextId, course, teacher, room, section);
    if (r == ADD_OK) {
        std::cout << "  Class added with id " << nextId << ".\n";
        nextId++;
    } else {
        std::cout << "  " << addResultText(r) << "\n";
    }
    return true;
}

static bool handlePlaceNext(Scheduler& s) {
    int front = -1;
    if (!s.getQueue().peek(front)) {
        std::cout << "  The waiting queue is empty.\n";
        return true;
    }
    ClassNode* c = s.getClasses().search(front);
    if (c != nullptr) {
        std::cout << "  Next in queue: " << c->id << " | " << c->course << " | "
                  << c->teacher << " | " << c->room << " | " << c->section << "\n";
    }

    int day = 0;
    int slot = 0;
    if (!readPosition(day, slot)) return false;

    int id = -1;
    PlaceResult res = PLACE_OK;
    s.placeNext(day, slot, id, res);
    std::cout << "  Class " << id << ": " << placeResultText(res) << "\n";
    return true;
}

static void handleSkip(Scheduler& s) {
    int id = -1;
    if (s.skipNext(id)) {
        std::cout << "  Class " << id << " sent to the back of the queue.\n";
    } else {
        std::cout << "  The waiting queue is empty.\n";
    }
}

static bool handleMove(Scheduler& s) {
    int id = 0;
    if (!readInt("  Class id: ", id)) return false;
    int day = 0;
    int slot = 0;
    if (!readPosition(day, slot)) return false;

    int oldDay = -1;
    int oldSlot = -1;
    PlaceResult res = s.moveClass(id, day, slot, oldDay, oldSlot);
    if (res == PLACE_OK) {
        std::cout << "  Class " << id << ": moved from " << positionText(oldDay, oldSlot)
                  << " to " << positionText(day, slot) << "\n";
    } else {
        std::cout << "  Class " << id << ": " << placeResultText(res) << "\n";
    }
    return true;
}

static void handleUndo(Scheduler& s) {
    UndoAction a = makePlaceAction(0, 0, 0);
    UndoResult r = s.undoLast(a);
    if (r == UNDO_NOTHING) {
        std::cout << "  Nothing to undo.\n";
    } else if (r == UNDO_BROKEN) {
        std::cout << "  Internal error while undoing.\n";
    } else if (a.type == ACTION_PLACE) {
        std::cout << "  Undone: class " << a.classId << " removed from "
                  << positionText(a.day, a.slot) << ", back at the front of the queue.\n";
    } else {
        std::cout << "  Undone: class " << a.classId << " moved back to "
                  << positionText(a.fromDay, a.fromSlot) << ".\n";
    }
}

static bool handleDelete(Scheduler& s) {
    int id = 0;
    if (!readInt("  Class id: ", id)) return false;
    DeleteResult r = s.deleteClass(id);
    std::cout << "  Class " << id << ": " << deleteResultText(r) << "\n";
    return true;
}

static bool handleUpdate(Scheduler& s) {
    int id = 0;
    std::string course;
    std::string teacher;
    std::string room;
    std::string section;
    if (!readInt("  Class id: ", id)) return false;
    if (!readText("  New course: ", course)) return false;
    if (!readText("  New teacher: ", teacher)) return false;
    if (!readText("  New room: ", room)) return false;
    if (!readText("  New section: ", section)) return false;

    UpdateResult r = s.updateClass(id, course, teacher, room, section);
    std::cout << "  Class " << id << ": " << updateResultText(r) << "\n";
    return true;
}

int main() {
    Scheduler scheduler(MAX_CLASSES);
    int nextId = 1;
    bool running = true;

    while (running) {
        printMenu();
        int choice = 0;
        if (!readInt("Choose an option: ", choice)) {
            break;
        }

        switch (choice) {
            case 1:  running = handleAdd(scheduler, nextId); break;
            case 2:  scheduler.getQueue().display(); break;
            case 3:  running = handlePlaceNext(scheduler); break;
            case 4:  handleSkip(scheduler); break;
            case 5:  running = handleMove(scheduler); break;
            case 6:  handleUndo(scheduler); break;
            case 7:  running = handleDelete(scheduler); break;
            case 8:  running = handleUpdate(scheduler); break;
            case 9:  scheduler.getRoutine().display(scheduler.getClasses()); break;
            case 10: scheduler.getRoutine().printGapReport(); break;
            case 11: scheduler.getClasses().display(); break;
            case 12: scheduler.getUndo().display(); break;
            case 0:
                std::cout << "Goodbye.\n";
                running = false;
                break;
            default:
                std::cout << "  Unknown option.\n";
        }
    }
    return 0;
}
