#!/bin/bash

echo "Testing Heredoc Implementation - Simple Tests"
echo "============================================="

# Test 1: Basic heredoc
echo "Test 1: Basic heredoc"
echo "cat << EOF
hello world
EOF" | ./minishell | tail -n 1
echo

# Test 2: Variable expansion
echo "Test 2: Variable expansion"
echo "cat << EOF
\$USER
EOF" | ./minishell | tail -n 1
echo

# Test 3: Quoted delimiter (no expansion)
echo "Test 3: Quoted delimiter (no expansion)"
echo "cat << 'EOF'
\$USER
EOF" | ./minishell | tail -n 1
echo

# Test 4: Heredoc with pipes
echo "Test 4: Heredoc with pipes"
echo "cat << EOF | grep hello
hello world
goodbye world
EOF" | ./minishell | tail -n 1
echo

# Test 5: Heredoc with redirection
echo "Test 5: Heredoc with redirection"
echo "cat << EOF > test_output.txt
redirected content
EOF
cat test_output.txt" | ./minishell | tail -n 1
rm -f test_output.txt
echo

# Test 6: Multiple variables
echo "Test 6: Multiple variables"
echo "cat << EOF
User: \$USER
Home: \$HOME
EOF" | ./minishell | tail -n 2
echo

echo "All tests completed!" 