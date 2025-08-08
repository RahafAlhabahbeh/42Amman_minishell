#!/bin/bash

run_bash() {
  echo "Bash output:"
  bash -c "$1"
  echo "Exit status: $?"
  echo
}

run_minishell() {
  echo "Minishell output:"
  # Run minishell, execute command, then exit shell
  echo -e "$1\nexit" | ./minishell
  local status=$?
  echo "Exit status: $status"
  echo
  return $status
}

test_cmd() {
  local cmd="$1"
  local desc="$2"

  echo "=== Test: $desc ==="
  echo "Command(s): $cmd"

  # Replace ';' by newlines for bash commands
  local bash_cmd=$(echo -e "$cmd" | tr ';' '\n')

  if [[ "$cmd" =~ ^cd ]] || [[ "$cmd" =~ ^unset ]] || [[ "$cmd" =~ ^export ]]; then
    # For cd, unset, export - run bash then pwd and echo $PWD
    bash_output=$(bash -c "$bash_cmd; pwd; echo \$PWD" 2>&1)
    bash_status=$?
    echo "Bash output:"
    echo "$bash_output"
    echo "Exit status: $bash_status"
    echo

    # Prepare minishell input commands with newline separated
    minishell_input=$(echo -e "$cmd\npwd\necho \$PWD\nexit")
    minishell_output=$(echo -e "$minishell_input" | ./minishell 2>&1)
    minishell_status=$?
    echo "Minishell output:"
    echo "$minishell_output"
    echo "Exit status: $minishell_status"
    echo

    # Remove minishell prompts and empty lines
    minishell_lines=$(echo "$minishell_output" | grep -vE '^minishell>' | sed '/^$/d')

    # Extract last two lines of output (pwd and echo $PWD) from both bash and minishell
    bash_lines=($(echo "$bash_output" | sed '/^$/d' | tail -n 2))
    minishell_lines_array=($(echo "$minishell_lines" | tail -n 2))

    if [[ "${bash_lines[0]}" == "${minishell_lines_array[0]}" && \
          "${bash_lines[1]}" == "${minishell_lines_array[1]}" && \
          $bash_status -eq $minishell_status ]]; then
      echo "Result: ✅ OK"
    else
      echo "Result: ❌ FAIL"
    fi
  else
    # For other commands, just run normally and compare exit statuses
    run_bash "$cmd"
    bash_status=$?
    run_minishell "$cmd"
    minishell_status=$?

    if [ $bash_status -eq $minishell_status ]; then
      echo "Result: ✅ OK"
    else
      echo "Result: ❌ FAIL"
    fi
  fi

  echo "-------------------------"
  echo
}


# ===========================
# CD COMMAND TESTS
# ===========================

test_cmd "cd ../../../../../.." "CD to root (should be /)"
test_cmd "cd ~" "CD home"
test_cmd "cd" "CD no args"
test_cmd "cd ~/Desktop" "CD to Desktop"
test_cmd "unset HOME\ncd ~" "CD home with HOME unset (expect error)"
test_cmd "export HOME=/Users/user42\ncd ~" "CD home with custom HOME"
test_cmd "cd .." "CD up one directory"
test_cmd "cd Eyooooo" "CD invalid directory"
test_cmd "cd -" "CD to last PWD"

# ===========================
# ECHO COMMAND TESTS
# ===========================

test_cmd "echo" "echo empty"
test_cmd "echo \$NonExistingVar" "echo unset var"
test_cmd "echo \$PATH" "echo PATH var"
test_cmd "echo ~" "echo ~ tilde expansion"
test_cmd "echo -n -n -nnnn -nnnnm" "echo multiple -n flags"
test_cmd "echo -n -nnn hello -n" "echo with -n and text"

# ===========================
# ENV COMMAND TESTS
# ===========================

test_cmd "env" "env list"
test_cmd "env Weeiirrddd" "env with invalid arg"

# ===========================
# EXIT STATUS TESTS
# ===========================

test_cmd "cd Weyooo" "cd invalid dir (expect error)"
test_cmd "echo \$?" "Check exit status from last command"
test_cmd "random_cmd" "command not found exit status"
test_cmd "echo \$?" "Check exit status"
test_cmd "./file_that_is_not_executable" "file not executable exit status"
test_cmd "echo \$?" "Check exit status"
test_cmd "cat bla" "cat no such file exit status"
test_cmd "echo \$?" "Check exit status"
test_cmd "hi" "command not found exit status"
test_cmd "echo \$?" "Check exit status"
test_cmd "export ls='ls -l'" "export and run var"
test_cmd "\$ls" "run exported ls alias"
test_cmd "echo \$?" "Check exit status"

# ===========================
# EXPORT TESTS
# ===========================

test_cmd 'export _P1 _P2= _P3="" _P4=" " _P5 _P6=6 _P7="7"' "all export cases"
test_cmd "export var=a" "export var"
test_cmd "export \$var=test" "export using var as var name"
test_cmd "echo \$var \$a" "echo exported variables"
test_cmd "export test1" "export var without value"
test_cmd "env | grep test1" "check env list"
test_cmd "export test2=" "export var empty string"
test_cmd "env | grep test2" "check env list"
test_cmd "export var" "export var"
test_cmd "export var=hoi" "overwrite var"
test_cmd "export" "show export list"
test_cmd "echo \$var" "echo var"
test_cmd "export '' test=a" "export invalid empty var"

# ===========================
# PARSING & SUBSTITUTION
# ===========================

test_cmd 'echo "\s"' "echo backslash in quotes"
test_cmd 'echo "\\s"' "echo double backslash in quotes"
test_cmd 'echo "12\""' "echo escaped quote"
test_cmd 'echo $USER$var$USER$USER$USERtest$USER' "multiple var substitution"
test_cmd 'echo bonjour \; ls' "escaped semicolon"
test_cmd 'echo \>' "escaped >"

# ===========================
# PIPE TESTS
# ===========================

test_cmd "cat Makefile | grep pr | head -n 5 | hello" "pipe with invalid command"
test_cmd "ls | exit" "pipe with exit"

# ===========================
# REDIRECTION TESTS
# ===========================

test_cmd "echo blabla > test" "redirect output to file"
test_cmd "exit > hoi.txt" "exit and redirect output"
test_cmd "cd .. > derp.txt" "redirect cd output (should not change dir)"
test_cmd "cat -e > test1 < test2" "redirect input non-existing file"
test_cmd "echo 2 > out1 >> out2" "multiple redirections"
test_cmd "echo 2 >> out1 > out2" "multiple redirections"
test_cmd "echo test > file test1" "redirection with extra arg"

# ===========================
# SIGNALS & SPECIAL
# ===========================

echo "For signal tests (Ctrl+C, Ctrl+\\, Ctrl+D), please test manually."

# ===========================
# UNSET TESTS
# ===========================

test_cmd "unset PATH" "unset PATH"
test_cmd "echo \$PATH" "echo PATH after unset"
test_cmd "unset PATH\nls" "unset PATH then ls"
test_cmd "unset '' test" "unset invalid variable"
test_cmd "unset =" "unset invalid variable"
test_cmd "unset PWD" "unset PWD"

echo "All tests completed."
