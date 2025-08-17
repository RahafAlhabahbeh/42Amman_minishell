#!/bin/bash

echo "=== Testing Pipes and Redirections ==="

echo "Test 1: Simple pipe"
echo "Expected: List of files containing 'Makefile', exit status 0"
echo "ls | grep Makefile" | ./minishell
echo "Exit status: $?"
echo

echo "Test 2: Output redirection"
echo "Expected: Create test_output.txt with 'hello', exit status 0"
echo "echo hello > test_output.txt" | ./minishell
echo "Contents of test_output.txt:"
cat test_output.txt 2>/dev/null || echo "File not found"
echo "Exit status: $?"
echo

echo "Test 3: Input redirection"
echo "Expected: Read from test_output.txt, exit status 0"
echo "cat < test_output.txt" | ./minishell
echo "Exit status: $?"
echo

echo "Test 4: Append redirection"
echo "Expected: Append 'world' to test_output.txt, exit status 0"
echo "echo world >> test_output.txt" | ./minishell
echo "Contents of test_output.txt after append:"
cat test_output.txt 2>/dev/null || echo "File not found"
echo "Exit status: $?"
echo

echo "Test 5: Multiple pipes"
echo "Expected: Process through multiple commands, exit status 0"
echo "echo hello world | cat | cat" | ./minishell
echo "Exit status: $?"
echo

# Cleanup
rm -f test_output.txt

