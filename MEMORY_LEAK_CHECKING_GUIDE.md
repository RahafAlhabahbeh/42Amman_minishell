# Memory Leak Checking Guide for Minishell

## Overview
This guide provides a comprehensive step-by-step approach to detect and fix memory leaks in your minishell project using Valgrind and other tools.

## Prerequisites
Make sure you have the following tools installed:
```bash
# Install Valgrind if not already installed
sudo apt-get install valgrind

# Check if Valgrind is installed
valgrind --version
```

## Step-by-Step Memory Leak Detection

### Step 1: Clean Build
Always start with a clean build to ensure you're testing the latest code:
```bash
# Clean previous builds
make clean

# Rebuild the project
make

# Verify the executable was created
ls -la minishell
```

### Step 2: Basic Valgrind Test
Start with a simple test to ensure Valgrind is working:
```bash
# Test with a simple command
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_basic.txt ./minishell

# In the minishell, run a simple command
echo "hello world"
exit
```

### Step 3: Test with Heredoc (Critical Area)
Since your project has heredoc implementation, test it thoroughly:
```bash
# Test heredoc functionality
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_heredoc.txt ./minishell

# In the minishell, test heredoc
cat << EOF
hello world
EOF
exit
```

### Step 4: Test with Variable Expansion
Test memory management with variable expansion:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_vars.txt ./minishell

# In the minishell, test variable expansion
echo $USER
echo $HOME
cat << EOF
$USER $HOME
EOF
exit
```

### Step 5: Test with Pipes and Redirections
Test complex scenarios that might cause leaks:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_pipes.txt ./minishell

# In the minishell, test pipes and redirections
echo "test" | grep test
cat << EOF > testfile.txt
content
EOF
cat testfile.txt
rm testfile.txt
exit
```

### Step 6: Test Signal Handling
Test memory management during signal handling:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_signals.txt ./minishell

# In the minishell, test signal handling
# Start a heredoc and press Ctrl+C
cat << EOF
# Press Ctrl+C here
EOF
exit
```

### Step 7: Test Builtin Commands
Test all builtin commands for memory leaks:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_builtins.txt ./minishell

# In the minishell, test builtins
cd /tmp
pwd
export TEST_VAR=value
env | grep TEST_VAR
unset TEST_VAR
exit
```

### Step 8: Test Error Conditions
Test memory management during error conditions:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_errors.txt ./minishell

# In the minishell, test error conditions
cat nonexistent_file
echo $NONEXISTENT_VAR
exit
```

### Step 9: Test Multiple Commands
Test with multiple commands in sequence:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind_multiple.txt ./minishell

# In the minishell, test multiple commands
echo "first" && echo "second" || echo "third"
ls -la | grep "\.c" | wc -l
exit
```

### Step 10: Test with Suppressions
Use the existing readline suppressions file:
```bash
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --suppressions=readline.supp --log-file=valgrind_suppressed.txt ./minishell

# In the minishell, test normal usage
echo "hello"
ls
exit
```

## Analyzing Valgrind Output

### Step 11: Check for Memory Leaks
After each test, check the valgrind output:
```bash
# Look for the HEAP SUMMARY section
grep -A 10 "HEAP SUMMARY" valgrind_*.txt

# Look for "definitely lost" leaks
grep -i "definitely lost" valgrind_*.txt

# Look for "indirectly lost" leaks
grep -i "indirectly lost" valgrind_*.txt

# Look for "possibly lost" leaks
grep -i "possibly lost" valgrind_*.txt
```

### Step 12: Check for Memory Errors
Look for other memory errors:
```bash
# Check for invalid reads/writes
grep -i "invalid read\|invalid write" valgrind_*.txt

# Check for use after free
grep -i "use after free" valgrind_*.txt

# Check for double free
grep -i "double free" valgrind_*.txt
```

## Automated Testing Script

### Step 13: Create Automated Test Script
Create a comprehensive test script:
```bash
#!/bin/bash

# Create test script
cat > memory_test.sh << 'EOF'
#!/bin/bash

echo "=== Memory Leak Testing Script ==="

# Clean and rebuild
echo "1. Cleaning and rebuilding..."
make clean
make

# Test scenarios
test_scenarios=(
    "basic:echo 'hello world'"
    "heredoc:cat << EOF\nhello\nEOF"
    "variables:echo \$USER"
    "pipes:echo 'test' | grep test"
    "builtins:cd /tmp && pwd"
    "errors:cat nonexistent_file"
    "multiple:echo 'first' && echo 'second'"
)

for scenario in "${test_scenarios[@]}"; do
    name=$(echo $scenario | cut -d: -f1)
    command=$(echo $scenario | cut -d: -f2-)
    
    echo "Testing: $name"
    echo "$command" | valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file="valgrind_${name}.txt" ./minishell > /dev/null 2>&1
    
    # Check for leaks
    leaks=$(grep -c "definitely lost\|indirectly lost\|possibly lost" "valgrind_${name}.txt" || echo "0")
    if [ "$leaks" -gt 0 ]; then
        echo "  ❌ Memory leaks detected in $name"
        grep -i "definitely lost\|indirectly lost\|possibly lost" "valgrind_${name}.txt"
    else
        echo "  ✅ No memory leaks in $name"
    fi
done

echo "=== Testing Complete ==="
EOF

chmod +x memory_test.sh
```

### Step 14: Run Automated Tests
```bash
# Run the automated test script
./memory_test.sh

# Check all results at once
echo "=== Summary of All Tests ==="
for file in valgrind_*.txt; do
    echo "=== $file ==="
    grep -A 5 "HEAP SUMMARY" "$file"
    echo
done
```

## Common Memory Leak Sources

### Step 15: Check These Common Areas
Look for leaks in these specific areas:

1. **Command Parsing**
   - Check `srcs/minilib/parsing/` files
   - Look for unfreed command structures

2. **Redirection Handling**
   - Check `srcs/minilib/redirection/` files
   - Look for unfreed file descriptors

3. **Heredoc Implementation**
   - Check `srcs/minilib/redirection/heredoc.c`
   - Look for unfreed temporary files

4. **Environment Variables**
   - Check environment variable handling
   - Look for unfreed variable strings

5. **Signal Handling**
   - Check `srcs/minilib/signal/` files
   - Look for memory allocated during signal handling

## Fixing Memory Leaks

### Step 16: Identify and Fix Leaks
When you find leaks:

1. **Locate the source**: Use the stack trace in Valgrind output
2. **Add proper cleanup**: Ensure all allocated memory is freed
3. **Test again**: Re-run the specific test that showed the leak
4. **Verify fix**: Ensure no new leaks are introduced

### Step 17: Common Fixes
```c
// Example of proper memory management
char *str = malloc(100);
if (str == NULL) {
    // Handle allocation failure
    return NULL;
}
// Use str
free(str);  // Always free allocated memory
str = NULL; // Set to NULL after freeing
```

## Final Verification

### Step 18: Comprehensive Final Test
```bash
# Run a comprehensive final test
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --log-file=valgrind_final.txt ./minishell

# In the minishell, run a comprehensive test sequence
echo "Starting comprehensive test"
export TEST_VAR=value
echo $TEST_VAR
cat << EOF
This is a heredoc test
with $TEST_VAR expansion
EOF
echo "test" | grep test
cd /tmp && pwd
unset TEST_VAR
echo "Test complete"
exit
```

### Step 19: Check Final Results
```bash
# Check the final results
echo "=== Final Memory Leak Check ==="
grep -A 10 "HEAP SUMMARY" valgrind_final.txt
grep -i "definitely lost\|indirectly lost\|possibly lost" valgrind_final.txt || echo "No memory leaks detected!"
```

## Tips and Best Practices

1. **Always test with Valgrind** before submitting
2. **Use suppressions** for known library leaks (like readline)
3. **Test edge cases** and error conditions
4. **Check for leaks after each major feature implementation**
5. **Keep track of all allocated memory** in your code
6. **Use consistent memory management patterns**

## Quick Commands Reference

```bash
# Quick memory leak check
valgrind --leak-check=full --show-leak-kinds=all ./minishell

# Check for specific types of leaks
valgrind --leak-check=full --show-leak-kinds=definite,indirect ./minishell

# Check with suppressions
valgrind --leak-check=full --suppressions=readline.supp ./minishell

# Check for memory errors (not just leaks)
valgrind --tool=memcheck --track-origins=yes ./minishell
```

This comprehensive guide should help you systematically check for and fix memory leaks in your minishell project. 