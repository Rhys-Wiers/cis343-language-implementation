#!/usr/bin/env bash

# Exit immediately if a command fails during build
set -e

# Configurable paths
EXE="./build/hashbrown"
TEST_DIR="./test/lab5"

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

echo "=== Running Hashbrown Interpreter Test Suite ==="
echo ""

# Allow non-zero exit codes during test execution
set +e

PASSED=0
FAILED=0

for test_file in "$TEST_DIR"/*.hbn; do
    echo "----------------------------------------"
    echo "Running: $test_file"
    echo "----------------------------------------"
    
    "$EXE" "$test_file"
    EXIT_CODE=$?
    
    if [[ "$(basename "$test_file")" == error-* ]]; then
        if [ $EXIT_CODE -ne 0 ]; then
            echo -e "\033[32m[PASSED (Expected Error)]\033[0m Exit code: $EXIT_CODE"
            PASSED=$((PASSED + 1))
        else
            echo -e "\033[31m[FAILED (Expected Error, Got 0)]\033[0m Exit code: $EXIT_CODE"
            FAILED=$((FAILED + 1))
        fi
    else
        if [ $EXIT_CODE -eq 0 ]; then
            echo -e "\033[32m[PASSED]\033[0m Exit code: $EXIT_CODE"
            PASSED=$((PASSED + 1))
        else
            echo -e "\033[31m[FAILED]\033[0m Exit code: $EXIT_CODE"
            FAILED=$((FAILED + 1))
        fi
    fi
    echo ""
done

echo "========================================"
echo "Summary: $PASSED passed, $FAILED failed."
echo "========================================"

if [ $FAILED -gt 0 ]; then
    exit 1
fi
