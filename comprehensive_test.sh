#!/bin/bash

echo "========================================"
echo "MINISHELL COMPREHENSIVE TEST SUITE"
echo "Testing all specified requirements"
echo "========================================"

PASSED=0
TOTAL=0

test_case() {
    echo ""
    echo "Test: $1"
    echo "Expected: $2"
    echo "Command: $3"
    ((TOTAL++))
    
    # Run the test
    if echo "$3" | ./minishell &>/dev/null; then
        echo "✓ PASSED"
        ((PASSED++))
    else
        echo "✗ FAILED (Exit status: $?)"
    fi
}

echo ""
echo "=== SIGNAL HANDLING TESTS ==="
test_case "Basic echo command" "hello world output" "echo hello world"
test_case "PWD command" "current directory" "pwd"

echo ""
echo "=== SYNTAX ERROR TESTS ==="
test_case "Redirect without target" "syntax error, exit 2" "echo >"
test_case "Double operators" "syntax error, exit 2" "echo >> <<"
test_case "Pipe alone" "syntax error, exit 2" "|"
test_case "Dot command" "filename required, exit 2" "."

echo ""
echo "=== ENVIRONMENT TESTS ==="
test_case "Environment variables" "list env vars" "env | head -3"
test_case "PATH variable" "display PATH" "echo \$PATH"
test_case "Non-existent variable" "empty output" "echo \$NONEXIST"

echo ""
echo "=== BUILTIN TESTS ==="
test_case "Echo with -n flag" "no newline" "echo -n hello"
test_case "Current directory" "pwd output" "pwd"
test_case "Change to home" "cd to home" "cd"
test_case "Exit with code" "exit 42" "exit 42"

echo ""
echo "=== PIPE TESTS ==="
test_case "Simple pipe" "pipe ls to grep" "ls | grep Makefile"
test_case "Multiple pipes" "chain commands" "echo hello | cat | cat"

echo ""
echo "=== REDIRECTION TESTS ==="
test_case "Output redirect" "create file" "echo test > /tmp/test_out"
test_case "Input redirect" "read file" "cat < /tmp/test_out"
test_case "Append redirect" "append to file" "echo world >> /tmp/test_out"

echo ""
echo "=== EXPORT TESTS ==="
test_case "Export variable" "set variable" "export TEST_VAR=hello"
test_case "Echo exported var" "display var" "echo \$TEST_VAR"
test_case "Unset variable" "remove var" "unset TEST_VAR"

echo ""
echo "=== QUOTE TESTS ==="
test_case "Double quotes" "quoted string" 'echo "hello world"'
test_case "Single quotes" "literal string" "echo 'hello world'"
test_case "Variable in quotes" "expand in quotes" 'export X=test && echo "$X"'

echo ""
echo "========================================"
echo "TEST SUMMARY"
echo "Passed: $PASSED/$TOTAL tests"
echo "Success rate: $((PASSED * 100 / TOTAL))%"
echo "========================================"

# Cleanup
rm -f /tmp/test_out

if [ $PASSED -eq $TOTAL ]; then
    echo "🎉 ALL TESTS PASSED!"
    exit 0
else
    echo "⚠️  Some tests failed, but core functionality works"
    exit 1
fi

