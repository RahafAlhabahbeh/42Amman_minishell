#!/bin/bash

echo "=== Testing Signal Handling ==="

echo "Test 1: CTRL+C with cat command"
echo "Expected: cat interrupted, exit status 130, back to prompt"
echo "cat" | timeout 5s ./minishell
echo "Exit status: $?"
echo

echo "Test 2: CTRL+D test"
echo "Expected: Exit shell with status 0"
echo "echo test" | ./minishell
echo "Exit status: $?"
echo

echo "Test 3: Basic echo test"
echo "Expected: hello world output, status 0"
echo "echo hello world" | ./minishell
echo "Exit status: $?"
echo

