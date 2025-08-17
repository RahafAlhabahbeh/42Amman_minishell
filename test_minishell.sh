#!/bin/bash

# Comprehensive test script for minishell based on provided test cases
# Usage: ./test_minishell.sh

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

test_result() {
    local test_name="$1"
    local expected="$2" 
    local actual="$3"
    local exit_code="$4"
    local expected_exit="$5"
    
    TOTAL=$((TOTAL + 1))
    
    if [[ "$actual" == *"$expected"* ]] && [[ "$exit_code" == "$expected_exit" ]]; then
        echo -e "${GREEN}[PASS]${NC} $test_name"
        PASSED=$((PASSED + 1))
    else
        echo -e "${RED}[FAIL]${NC} $test_name"
        echo "  Expected: $expected (exit: $expected_exit)"
        echo "  Actual: $actual (exit: $exit_code)"
        FAILED=$((FAILED + 1))
    fi
}

echo "=== Testing Basic Functionality ==="

# Test 1: Basic echo
result=$(echo "echo hello" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Basic echo" "hello" "$result" "$exit_code" "0"

# Test 2: Environment variable
result=$(echo "echo \$HOME" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Environment variable \$HOME" "$HOME" "$result" "$exit_code" "0"

# Test 3: Exit command
result=$(echo "exit" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Exit command" "exit" "$result" "$exit_code" "0"

echo -e "\n=== Testing Syntax Errors ==="

# Test 4: Command with redirect only
result=$(echo ">" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Syntax error: redirect only" "syntax error" "$result" "$exit_code" "2"

# Test 5: Double pipe
result=$(echo "||" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Syntax error: double pipe" "syntax error" "$result" "$exit_code" "2"

echo -e "\n=== Testing Echo Command ==="

# Test 6: Echo with -n flag
result=$(echo "echo -n test" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Echo with -n flag" "test" "$result" "$exit_code" "0"

# Test 7: Echo with multiple -n flags
result=$(echo "echo -n -nn -nnwn test" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Echo with multiple -n flags" "test" "$result" "$exit_code" "0"

echo -e "\n=== Testing Environment Variables ==="

# Test 8: Basic env command
result=$(echo "env" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Basic env command" "PATH" "$result" "$exit_code" "0"

# Test 9: Echo non-existent variable
result=$(echo "echo \$non_exist" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Echo non-existent variable" "" "$result" "$exit_code" "0"

echo -e "\n=== Testing Exit Command ==="

# Test 10: Exit with code
result=$(echo "exit 42" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Exit with code 42" "exit" "$result" "$exit_code" "42"

# Test 11: Exit with too many args
result=$(echo "exit 1 abcd" | timeout 5 ./minishell 2>&1)
exit_code=$?
test_result "Exit with too many args" "too many arguments" "$result" "$exit_code" "1"

echo -e "\n=== Summary ==="
echo -e "Total tests: $TOTAL"
echo -e "${GREEN}Passed: $PASSED${NC}"
echo -e "${RED}Failed: $FAILED${NC}"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
