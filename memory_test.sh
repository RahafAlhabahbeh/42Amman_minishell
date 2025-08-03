#!/bin/bash

echo "=== Memory Leak Testing Script for Minishell ==="
echo "This script will test various scenarios for memory leaks"
echo

# Check if minishell executable exists
if [ ! -f "./minishell" ]; then
    echo "❌ minishell executable not found. Building..."
    make clean
    make
    if [ ! -f "./minishell" ]; then
        echo "❌ Failed to build minishell. Exiting."
        exit 1
    fi
fi

# Check if readline.supp exists
if [ ! -f "./readline.supp" ]; then
    echo "⚠️  readline.supp not found. Creating basic suppressions..."
    cat > readline.supp << 'EOF'
{
    leak readline
    Memcheck:Leak
    ...
    fun:readline
}
{
    leak add_history
    Memcheck:Leak
    ...
    fun:add_history
}
EOF
fi

# Create output directory
mkdir -p valgrind_outputs

echo "1. Testing basic functionality..."
echo "echo 'hello world'" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/basic.txt ./minishell > /dev/null 2>&1

echo "2. Testing heredoc functionality..."
echo -e "cat << EOF\nhello world\nEOF" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/heredoc.txt ./minishell > /dev/null 2>&1

echo "3. Testing variable expansion..."
echo -e "echo \$USER\necho \$HOME" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/variables.txt ./minishell > /dev/null 2>&1

echo "4. Testing heredoc with variable expansion..."
echo -e "cat << EOF\n\$USER \$HOME\nEOF" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/heredoc_vars.txt ./minishell > /dev/null 2>&1

echo "5. Testing pipes..."
echo "echo 'test' | grep test" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/pipes.txt ./minishell > /dev/null 2>&1

echo "6. Testing redirections..."
echo -e "cat << EOF > testfile.txt\ncontent\nEOF\ncat testfile.txt\nrm testfile.txt" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/redirections.txt ./minishell > /dev/null 2>&1

echo "7. Testing builtin commands..."
echo -e "cd /tmp\npwd\nexport TEST_VAR=value\nenv | grep TEST_VAR\nunset TEST_VAR" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/builtins.txt ./minishell > /dev/null 2>&1

echo "8. Testing error conditions..."
echo -e "cat nonexistent_file\necho \$NONEXISTENT_VAR" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/errors.txt ./minishell > /dev/null 2>&1

echo "9. Testing multiple commands..."
echo -e "echo 'first' && echo 'second' || echo 'third'\nls -la | grep '\.c' | wc -l" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/multiple.txt ./minishell > /dev/null 2>&1

echo "10. Testing comprehensive scenario..."
echo -e "echo 'Starting comprehensive test'\nexport TEST_VAR=value\necho \$TEST_VAR\ncat << EOF\nThis is a heredoc test\nwith \$TEST_VAR expansion\nEOF\necho 'test' | grep test\ncd /tmp && pwd\nunset TEST_VAR\necho 'Test complete'" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_outputs/comprehensive.txt ./minishell > /dev/null 2>&1

echo
echo "=== Analyzing Results ==="
echo

# Function to check for leaks in a file
check_leaks() {
    local file=$1
    local test_name=$2
    
    echo "=== $test_name ==="
    
    # Check HEAP SUMMARY
    if grep -q "HEAP SUMMARY" "$file"; then
        echo "HEAP SUMMARY:"
        grep -A 5 "HEAP SUMMARY" "$file"
    else
        echo "No HEAP SUMMARY found"
    fi
    
    # Check for leaks
    local leaks=$(grep -c "definitely lost\|indirectly lost\|possibly lost" "$file" 2>/dev/null || echo "0")
    if [ "$leaks" -gt 0 ]; then
        echo "❌ Memory leaks detected:"
        grep -i "definitely lost\|indirectly lost\|possibly lost" "$file"
    else
        echo "✅ No memory leaks detected"
    fi
    
    # Check for other memory errors
    local errors=$(grep -c "invalid read\|invalid write\|use after free\|double free" "$file" 2>/dev/null || echo "0")
    if [ "$errors" -gt 0 ]; then
        echo "❌ Memory errors detected:"
        grep -i "invalid read\|invalid write\|use after free\|double free" "$file"
    else
        echo "✅ No memory errors detected"
    fi
    
    echo
}

# Check all test results
check_leaks "valgrind_outputs/basic.txt" "Basic Functionality"
check_leaks "valgrind_outputs/heredoc.txt" "Heredoc"
check_leaks "valgrind_outputs/variables.txt" "Variable Expansion"
check_leaks "valgrind_outputs/heredoc_vars.txt" "Heredoc with Variables"
check_leaks "valgrind_outputs/pipes.txt" "Pipes"
check_leaks "valgrind_outputs/redirections.txt" "Redirections"
check_leaks "valgrind_outputs/builtins.txt" "Builtin Commands"
check_leaks "valgrind_outputs/errors.txt" "Error Conditions"
check_leaks "valgrind_outputs/multiple.txt" "Multiple Commands"
check_leaks "valgrind_outputs/comprehensive.txt" "Comprehensive Test"

echo "=== Summary ==="
echo "All valgrind output files are saved in: valgrind_outputs/"
echo "To view detailed results, check individual files:"
echo "  cat valgrind_outputs/basic.txt"
echo "  cat valgrind_outputs/heredoc.txt"
echo "  etc."

# Count total issues
total_leaks=$(grep -r "definitely lost\|indirectly lost\|possibly lost" valgrind_outputs/ | wc -l)
total_errors=$(grep -r "invalid read\|invalid write\|use after free\|double free" valgrind_outputs/ | wc -l)

echo
echo "=== Final Count ==="
echo "Total memory leaks found: $total_leaks"
echo "Total memory errors found: $total_errors"

if [ "$total_leaks" -eq 0 ] && [ "$total_errors" -eq 0 ]; then
    echo "🎉 Congratulations! No memory issues detected!"
else
    echo "⚠️  Memory issues detected. Check the detailed output above."
fi

echo
echo "=== Quick Commands for Further Investigation ==="
echo "To check all leaks at once:"
echo "  grep -r 'definitely lost\|indirectly lost\|possibly lost' valgrind_outputs/"
echo
echo "To check all errors at once:"
echo "  grep -r 'invalid read\|invalid write\|use after free\|double free' valgrind_outputs/"
echo
echo "To view a specific test result:"
echo "  cat valgrind_outputs/[test_name].txt" 