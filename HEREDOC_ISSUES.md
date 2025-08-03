# Heredoc Implementation Issues and Fixes

## Current Issues Identified

### 1. Multiple Heredocs Not Working Correctly ❌
**Issue**: When you have `cat << 1 << 2 << 3`, the implementation processes all content instead of only the last heredoc.

**Expected Behavior**: Only output content from the last heredoc (delimiter "3")
**Current Behavior**: Outputs all content from all heredocs

**Fix Needed**: Process all heredocs in sequence, but only use content from the last one.

### 2. Delimiter Matching Issue ❌
**Issue**: In some cases, the heredoc stops at the first occurrence of the delimiter instead of the exact delimiter.

**Example**: 
```bash
cat << 1
111111111
1
     1
"1"
'1'
1
```

**Expected**: Should continue until the exact delimiter "1" at the end
**Current**: Stops at the first "1"

**Fix Needed**: Ensure exact delimiter matching.

### 3. Heredoc Without Command ❌
**Issue**: When you have just `<< 1` without a command, it should use `cat` as default.

**Expected Behavior**: `<< 1` should be equivalent to `cat << 1`
**Current Behavior**: Shows "command not found: (null)"

**Fix Needed**: Add default command handling.

### 4. Warning Message Format ❌
**Issue**: Warning message format doesn't match bash exactly.

**Expected**: `bash: warning: here-document at line delimited by end-of-file (wanted 'DELIMITER')`
**Current**: Format is slightly different

**Fix Needed**: Match bash warning message format exactly.

## Test Cases from User

### Working Correctly ✅
1. Basic heredoc with `<<` - ✅ Working
2. Variable expansion - ✅ Working  
3. Quoted delimiter (no expansion) - ✅ Working
4. Signal handling - ✅ Working
5. Integration with redirection and pipes - ✅ Working
6. Heredoc with builtin commands - ✅ Working
7. Chained with other commands - ✅ Working
8. Temporary file creation - ✅ Working
9. Error messages - ✅ Working (format needs minor fix)

### Needs Fixing ❌
1. Multiple heredocs in one line - ❌ Not working correctly
2. Delimiter matching edge cases - ❌ Some issues
3. Heredoc without command - ❌ Not working

## Implementation Plan

### Fix 1: Multiple Heredocs
- Modify command processing to handle multiple heredocs
- Process all heredocs in sequence
- Only use content from the last heredoc
- Update the command structure to track multiple heredocs

### Fix 2: Delimiter Matching
- Ensure exact delimiter matching
- Handle edge cases with special characters
- Test with various delimiter formats

### Fix 3: Default Command
- Add default command handling for heredoc without command
- Use `cat` as default when no command is specified

### Fix 4: Warning Message
- Match bash warning message format exactly
- Test with various edge cases

## Current Status
- Basic functionality: ✅ Working
- Edge cases: ❌ Need fixes
- Multiple heredocs: ❌ Need implementation
- Delimiter matching: ❌ Need improvements

## Next Steps
1. Fix multiple heredoc handling
2. Improve delimiter matching
3. Add default command handling
4. Fix warning message format
5. Comprehensive testing 