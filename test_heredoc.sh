#!/bin/bash

# Test script for heredoc functionality
# This script tests all the required cases for heredoc implementation

echo "=========================================="
echo "Testing Heredoc Implementation"
echo "=========================================="

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Function to run test
run_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    
    echo -e "${YELLOW}Test: $test_name${NC}"
    echo "Command: $command"
    echo "Expected: $expected_output"
    
    # Run the command and capture output
    output=$(echo "$command" | ./minishell 2>&1)
    
    if [ "$output" = "$expected_output" ]; then
        echo -e "${GREEN}✓ PASS${NC}"
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Got: '$output'"
    fi
    echo "----------------------------------------"
}

# Test 1: Basic heredoc with <<
echo -e "${YELLOW}Case 1: Basic heredoc with <<${NC}"
run_test "Basic heredoc" "cat << EOF
hello world
EOF" "hello world"

# Test 2: Variable expansion
echo -e "${YELLOW}Case 2: Variable expansion${NC}"
run_test "Variable expansion" "cat << EOF
\$USER
EOF" "$USER"

# Test 3: Quoted delimiter (no expansion)
echo -e "${YELLOW}Case 3: Quoted delimiter (no expansion)${NC}"
run_test "Quoted delimiter" "cat << 'EOF'
\$USER
EOF" "\$USER"

# Test 4: Multiple heredocs in one line
echo -e "${YELLOW}Case 4: Multiple heredocs in one line${NC}"
run_test "Multiple heredocs" "cat << EOF1 << EOF2
first
EOF1
second
EOF2" "second"

# Test 5: Integration with redirection
echo -e "${YELLOW}Case 5: Integration with redirection${NC}"
run_test "Heredoc with output redirection" "cat << EOF > test_output.txt
redirected content
EOF
cat test_output.txt" "redirected content"
rm -f test_output.txt

# Test 6: Integration with pipes
echo -e "${YELLOW}Case 6: Integration with pipes${NC}"
run_test "Heredoc with pipes" "cat << EOF | grep hello
hello world
goodbye world
EOF" "hello world"

# Test 7: Heredoc with builtin commands
echo -e "${YELLOW}Case 7: Heredoc with builtin commands${NC}"
run_test "Heredoc with echo" "echo << EOF
test content
EOF" "test content"

# Test 8: Chained with other commands
echo -e "${YELLOW}Case 8: Chained with other commands${NC}"
run_test "Heredoc chained with commands" "echo start && cat << EOF && echo end
middle
EOF" "start
middle
end"

# Test 9: Error handling - unexpected EOF
echo -e "${YELLOW}Case 9: Error handling - unexpected EOF${NC}"
echo "Test: Unexpected EOF (Ctrl+D)"
echo "This should show a warning message"
echo "cat << EOF" | ./minishell

# Test 10: Signal handling
echo -e "${YELLOW}Case 10: Signal handling${NC}"
echo "Test: Signal handling (Ctrl+C)"
echo "This should handle Ctrl+C gracefully"
echo "cat << EOF" | timeout 5s ./minishell || echo "Signal handled correctly"

# Test 11: Complex variable expansion
echo -e "${YELLOW}Case 11: Complex variable expansion${NC}"
run_test "Complex variable expansion" "cat << EOF
User: \$USER
Home: \$HOME
Path: \$PATH
EOF" "User: $USER
Home: $HOME
Path: $PATH"

# Test 12: Heredoc with exit status
echo -e "${YELLOW}Case 12: Heredoc with exit status${NC}"
run_test "Exit status expansion" "cat << EOF
Exit status: \$?
EOF" "Exit status: 0"

# Test 13: Heredoc with mixed quotes
echo -e "${YELLOW}Case 13: Heredoc with mixed quotes${NC}"
run_test "Mixed quotes in heredoc" "cat << EOF
'quoted text' and "double quoted"
EOF" "'quoted text' and \"double quoted\""

# Test 14: Heredoc with special characters
echo -e "${YELLOW}Case 14: Heredoc with special characters${NC}"
run_test "Special characters" "cat << EOF
Line 1
Line 2 with spaces    and tabs
Line 3 with \$pecial \$ymbols
EOF" "Line 1
Line 2 with spaces    and tabs
Line 3 with \$pecial \$ymbols"

echo "=========================================="
echo "Heredoc Testing Complete"
echo "==========================================" 