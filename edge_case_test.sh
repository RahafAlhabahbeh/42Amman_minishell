#!/bin/bash

echo "Testing Edge Cases from Bash Examples"
echo "====================================="

# Test 1: Basic heredoc with numeric delimiter
echo "Test 1: Basic heredoc with numeric delimiter"
echo "cat << 1
hdsfjk
hdsfjk
jdsfl
hfdsl
hfld
djd'
1" | ./minishell
echo

# Test 2: Heredoc with special characters in delimiter
echo "Test 2: Heredoc with special characters in delimiter"
echo "cat << 1
111111111
1
     1
\"1\"
'1'
1" | ./minishell
echo

# Test 3: Multiple heredocs
echo "Test 3: Multiple heredocs"
echo "cat << 1 << 2 << 3
11111111111
1
22222222222
2
33333333333
3" | ./minishell
echo

# Test 4: Heredoc without command
echo "Test 4: Heredoc without command"
echo "<< 1
1" | ./minishell
echo

# Test 5: Heredoc with variable expansion
echo "Test 5: Heredoc with variable expansion"
echo "cat << 1
\$USER
1" | ./minishell
echo

# Test 6: Heredoc with quoted delimiter
echo "Test 6: Heredoc with quoted delimiter"
echo "cat << \"1\"
\$USER
\"1\"" | ./minishell
echo

# Test 7: Heredoc with single quoted delimiter
echo "Test 7: Heredoc with single quoted delimiter"
echo "cat << '1'
\$USER
1" | ./minishell
echo

echo "Edge case tests completed!" 