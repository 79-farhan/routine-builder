#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <iostream>
#include <string>

// Tiny test helper: check() prints PASS or FAIL and counts both.
// Each test program includes this once, so plain static variables are enough.
static int testsRun = 0;
static int testsFailed = 0;

static void section(const std::string& title) {
    std::cout << "\n" << title << "\n";
}

static void check(bool ok, const std::string& name) {
    testsRun++;
    if (!ok) {
        testsFailed++;
    }
    std::cout << "  " << (ok ? "PASS" : "FAIL") << ": " << name << "\n";
}

// Prints the summary and returns the exit code: 0 when everything passed.
static int finishTests() {
    std::cout << "\n" << (testsRun - testsFailed) << " of " << testsRun
              << " checks passed\n";
    return testsFailed == 0 ? 0 : 1;
}

#endif
