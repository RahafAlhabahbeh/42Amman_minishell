#!/bin/bash

echo "=== Testing Syntax Errors ==="

echo "Test 1: Command + redirection operator alone"
echo "Expected: syntax error near unexpected token 'newline', exit status 2"
echo "echo >" | ./minishell
echo "Exit status: $?"
echo

echo "Test 2: Double operators"
echo "Expected: syntax error near unexpected token, exit status 2"
echo "echo >> <<" | ./minishell
echo "Exit status: $?"
echo

echo "Test 3: Pipe without command"
echo "Expected: syntax error near unexpected token 'newline', exit status 2"
echo "|" | ./minishell
echo "Exit status: $?"
echo

echo "Test 4: Dot command"
echo "Expected: filename argument required, exit status 2"
echo "." | ./minishell
echo "Exit status: $?"
echo

echo "Test 5: Double dot command"
echo "Expected: command not found, exit status 127"
echo ".." | ./minishell
echo "Exit status: $?"
echo

