#!/bin/bash

echo "=== Testing Environment Variables ==="

echo "Test 1: env command"
echo "Expected: Print all environment variables, exit status 0"
echo "env | head -5" | ./minishell
echo "Exit status: $?"
echo

echo "Test 2: echo \$PATH"
echo "Expected: Print PATH variable, exit status 0"
echo "echo \$PATH" | ./minishell
echo "Exit status: $?"
echo

echo "Test 3: echo \$non_exist"
echo "Expected: Print newline only, exit status 0"
echo "echo \$non_exist" | ./minishell
echo "Exit status: $?"
echo

echo "Test 4: echo \$SHLVL"
echo "Expected: Print current shell level, exit status 0"
echo "echo \$SHLVL" | ./minishell
echo "Exit status: $?"
echo

echo "Test 5: cd && echo \$PWD"
echo "Expected: Print current working directory, exit status 0"
echo -e "cd\necho \$PWD" | ./minishell
echo "Exit status: $?"
echo

