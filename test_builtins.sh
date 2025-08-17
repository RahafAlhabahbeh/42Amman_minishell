#!/bin/bash

echo "=== Testing Builtin Commands ==="

echo "Test 1: echo with -n flag"
echo "Expected: No newline after output, exit status 0"
echo "echo -n hello world" | ./minishell
echo "Exit status: $?"
echo

echo "Test 2: echo with multiple -n flags"
echo "Expected: No newline, exit status 0"
echo "echo -n -nn -nnwn test" | ./minishell
echo "Exit status: $?"
echo

echo "Test 3: pwd"
echo "Expected: Print current directory, exit status 0"
echo "pwd" | ./minishell
echo "Exit status: $?"
echo

echo "Test 4: cd to home"
echo "Expected: Change to home directory, exit status 0"
echo -e "cd\npwd" | ./minishell
echo "Exit status: $?"
echo

echo "Test 5: cd with invalid directory"
echo "Expected: Error message, exit status 1"
echo "cd /non_existent_directory" | ./minishell
echo "Exit status: $?"
echo

echo "Test 6: exit with number"
echo "Expected: Exit with status 42"
echo "exit 42" | ./minishell
echo "Exit status: $?"
echo

