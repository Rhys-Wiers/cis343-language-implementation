#!/usr/bin/env bash

# Exit immediately if a command fails during build
set -e

# Configurable paths
EXE="./build/hashbrown"
TEST_DIR="./test/lab4"

# Rebuild project if using Makefile or CMake
if [ -f "Makefile" ]; then
    echo "=== Building project ==="
    make
    echo ""
fi

if [ ! -f "$EXE" ]; then
    echo "Error: Executable $EXE not found!"
    exit 1
fi

echo "=== Running Lox Interpreter Test Suite ==="
echo ""

PASSED=0
FAILED=0

for test_file in "$TEST_DIR"/*.hbn; do
    echo "----------------------------------------"
    echo "Running: $test_file"
    echo "----------------------------------------"
    
    # Execute file through Lox interpreter
    "$EXE" "$test_file"
    EXIT_CODE=$?
    
    if [ $EXIT_CODE -eq 0 ]; then
        echo -e "\031[32m[PASSED]\033[0m Exit code: $EXIT_CODE"
        PASSED=$((PASSED + 1))
    else
        echo -e "\033[31m[FAILED]\033[0m Exit code: $EXIT_CODE"
        FAILED=$((FAILED + 1))
    fi
    echo ""
done

echo "========================================"
echo "Summary: $PASSED passed, $FAILED failed."
echo "========================================"

if [ $FAILED -gt 0 ]; then
    exit 1
fi
