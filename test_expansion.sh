#!/bin/bash

echo "=== Testing Variable Expansion and Export ==="

echo "Test 1: export and echo variable"
echo "Expected: Set and display variable, exit status 0"
echo -e "export test_var=hello\necho \$test_var" | ./minishell
echo "Exit status: $?"
echo

echo "Test 2: export multiple variables"
echo "Expected: Set multiple variables, exit status 0"
echo -e "export x1=abc x2=123\nexport\nexit" | ./minishell | grep -E "(x1|x2)" | head -2
echo "Exit status: $?"
echo

echo "Test 3: unset variable"
echo "Expected: Remove variable, exit status 0"
echo -e "export test_var=hello\nunset test_var\necho \$test_var" | ./minishell
echo "Exit status: $?"
echo

echo "Test 4: export with quotes"
echo "Expected: Handle quoted values, exit status 0"
echo -e "export test_var=\"hello world\"\necho \$test_var" | ./minishell
echo "Exit status: $?"
echo

echo "Test 5: echo with quotes"
echo "Expected: Handle quoted strings properly, exit status 0"
echo "echo \"hello world\"" | ./minishell
echo "Exit status: $?"
echo

echo "Test 6: Variable substitution in quotes"
echo "Expected: Expand variables in double quotes, exit status 0"
echo -e "export name=world\necho \"hello \$name\"" | ./minishell
echo "Exit status: $?"
echo

