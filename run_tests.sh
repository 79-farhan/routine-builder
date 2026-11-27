#!/bin/bash
# Builds and runs every test program in tests/. Run from the project root:
#   bash run_tests.sh
CXX="g++ -std=c++17 -Wall -Wextra -Isrc"
failed=0

run_test() {
    name=$1
    shift
    echo "=== $name ==="
    if ! $CXX -o "tests/$name" "tests/$name.cpp" "$@"; then
        echo "BUILD FAILED: $name"
        failed=1
        return
    fi
    if ! "./tests/$name"; then
        failed=1
    fi
    echo
}

run_test test_classlist src/ClassList.cpp
run_test test_queue src/ClassQueue.cpp
run_test test_stack src/UndoStack.cpp
run_test test_sessions src/ClassList.cpp

if [ $failed -eq 0 ]; then
    echo "ALL TEST PROGRAMS PASSED"
else
    echo "SOME TESTS FAILED"
    exit 1
fi
