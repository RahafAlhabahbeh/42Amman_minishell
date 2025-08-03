# Quick Memory Leak Check Reference

## 🚀 Fast Commands (Copy & Paste)

### 1. Quick Basic Check
```bash
valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
```

### 2. Run Automated Test Suite
```bash
./memory_test.sh
```

### 3. Test Specific Scenario
```bash
# Test heredoc (most critical)
echo -e "cat << EOF\nhello\nEOF" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

# Test variables
echo "echo \$USER" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

# Test pipes
echo "echo 'test' | grep test" | valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell
```

## 🔍 What to Look For

### ✅ Good Output (No Leaks)
```
==XXXX== HEAP SUMMARY:
==XXXX==     in use at exit: 0 bytes in 0 blocks
==XXXX==   total heap usage: XXX allocs, XXX frees, XXX bytes allocated
==XXXX== 
==XXXX== All heap blocks were freed -- no leaks are possible
```

### ❌ Bad Output (Leaks Detected)
```
==XXXX== HEAP SUMMARY:
==XXXX==     in use at exit: 72,704 bytes in 1 blocks
==XXXX==   total heap usage: 1,234 allocs, 1,233 frees, 123,456 bytes allocated
==XXXX== 
==XXXX== 72,704 bytes in 1 blocks are definitely lost in loss record 1 of 1
```

## 🎯 Critical Areas to Test

1. **Heredoc** - Most likely source of leaks
2. **Variable Expansion** - Environment variables
3. **Pipes** - File descriptor management
4. **Builtin Commands** - cd, export, unset, etc.
5. **Error Conditions** - Invalid commands, missing files

## 📋 Step-by-Step Process

1. **Clean Build**
   ```bash
   make clean && make
   ```

2. **Run Automated Test**
   ```bash
   ./memory_test.sh
   ```

3. **Check Results**
   ```bash
   grep -r "definitely lost\|indirectly lost\|possibly lost" valgrind_outputs/
   ```

4. **Fix Issues** (if any found)
   - Look at stack traces in valgrind output
   - Add proper `free()` calls
   - Test again

## 🛠️ Common Fixes

### Memory Allocation Pattern
```c
// ❌ Bad
char *str = malloc(100);
// use str
// forget to free

// ✅ Good
char *str = malloc(100);
if (str == NULL) {
    return NULL; // handle allocation failure
}
// use str
free(str);
str = NULL; // prevent use after free
```

### File Descriptor Pattern
```c
// ❌ Bad
int fd = open("file.txt", O_RDONLY);
// use fd
// forget to close

// ✅ Good
int fd = open("file.txt", O_RDONLY);
if (fd == -1) {
    return -1; // handle open failure
}
// use fd
close(fd);
```

## 🚨 Emergency Commands

### Check All Leaks at Once
```bash
grep -r "definitely lost\|indirectly lost\|possibly lost" valgrind_outputs/
```

### Check All Errors at Once
```bash
grep -r "invalid read\|invalid write\|use after free\|double free" valgrind_outputs/
```

### View Specific Test Result
```bash
cat valgrind_outputs/heredoc.txt
```

## 📝 Notes

- **Always use `--suppressions=readline.supp`** to ignore readline library leaks
- **Test with `exit` command** to ensure proper cleanup
- **Check both normal and error paths** in your code
- **Focus on heredoc first** - it's the most complex feature
- **Test edge cases** - empty commands, invalid syntax, etc.

## 🎯 Priority Order

1. **Heredoc leaks** (highest priority)
2. **Variable expansion leaks**
3. **Pipe/redirection leaks**
4. **Builtin command leaks**
5. **General memory management**

---

**Remember**: A clean valgrind output is required for passing the project! 🎓 