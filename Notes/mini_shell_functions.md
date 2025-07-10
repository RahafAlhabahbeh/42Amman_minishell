# UNIX Functions Reference for Minishell

## 1. `readline`
* **Required Library**: `<readline/readline.h>`
* **Prototype**: `char *readline(const char *prompt);`
* **Parameters**: `prompt` - string to display as prompt
* **Return Value**: Returns a line of text with the trailing newline removed, or NULL on EOF
* **Usage in Minishell**: Reading user input commands with a prompt

## 2. `rl_clear_history`
* **Required Library**: `<readline/history.h>`
* **Prototype**: `void rl_clear_history(void);`
* **Parameters**: None
* **Return Value**: None
* **Usage in Minishell**: Clearing the command history

## 3. `rl_on_new_line`
* **Required Library**: `<readline/readline.h>`
* **Prototype**: `int rl_on_new_line(void);`
* **Parameters**: None
* **Return Value**: 0 on success
* **Usage in Minishell**: Telling the update functions that we have moved onto a new line

## 4. `rl_replace_line`
* **Required Library**: `<readline/readline.h>`
* **Prototype**: `void rl_replace_line(const char *text, int clear_undo);`
* **Parameters**: `text` - new contents, `clear_undo` - boolean to clear undo list
* **Return Value**: None
* **Usage in Minishell**: Replacing the contents of the current line

## 5. `rl_redisplay`
* **Required Library**: `<readline/readline.h>`
* **Prototype**: `void rl_redisplay(void);`
* **Parameters**: None
* **Return Value**: None
* **Usage in Minishell**: Refreshing the current line after content changes

## 6. `add_history`
* **Required Library**: `<readline/history.h>`
* **Prototype**: `void add_history(const char *line);`
* **Parameters**: `line` - command to add to history
* **Return Value**: None
* **Usage in Minishell**: Adding user commands to history for recall

## 7. `printf`
* **Required Library**: `<stdio.h>`
* **Prototype**: `int printf(const char *format, ...);`
* **Parameters**: `format` - format string, followed by arguments
* **Return Value**: Number of characters printed or negative value on error
* **Usage in Minishell**: Displaying formatted output to the user

## 8. `malloc`
* **Required Library**: `<stdlib.h>`
* **Prototype**: `void *malloc(size_t size);`
* **Parameters**: `size` - number of bytes to allocate
* **Return Value**: Pointer to allocated memory or NULL on failure
* **Usage in Minishell**: Dynamic memory allocation for shell data structures

## 9. `free`
* **Required Library**: `<stdlib.h>`
* **Prototype**: `void free(void *ptr);`
* **Parameters**: `ptr` - pointer to memory to free
* **Return Value**: None
* **Usage in Minishell**: Releasing allocated memory to prevent leaks

## 10. `write`
* **Required Library**: `<unistd.h>`
* **Prototype**: `ssize_t write(int fd, const void *buf, size_t count);`
* **Parameters**: `fd` - file descriptor, `buf` - buffer to write, `count` - bytes to write
* **Return Value**: Number of bytes written or -1 on error
* **Usage in Minishell**: Writing output to standard output or files

## 11. `access`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int access(const char *pathname, int mode);`
* **Parameters**: `pathname` - path to check, `mode` - access mode to check for
* **Return Value**: 0 on success (access granted), -1 on error
* **Usage in Minishell**: Checking if files exist or are executable

## 12. `open`
* **Required Library**: `<fcntl.h>`
* **Prototype**: `int open(const char *pathname, int flags, mode_t mode);`
* **Parameters**: `pathname` - path to file, `flags` - access mode, `mode` - permissions
* **Return Value**: File descriptor or -1 on error
* **Usage in Minishell**: Opening files for redirection

## 13. `read`
* **Required Library**: `<unistd.h>`
* **Prototype**: `ssize_t read(int fd, void *buf, size_t count);`
* **Parameters**: `fd` - file descriptor, `buf` - buffer to read into, `count` - bytes to read
* **Return Value**: Number of bytes read, 0 on EOF, or -1 on error
* **Usage in Minishell**: Reading from files or pipes

## 14. `close`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int close(int fd);`
* **Parameters**: `fd` - file descriptor to close
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Closing file descriptors after use

## 15. `fork`
* **Required Library**: `<unistd.h>`
* **Prototype**: `pid_t fork(void);`
* **Parameters**: None
* **Return Value**: 0 in child process, PID of child in parent, -1 on error
* **Usage in Minishell**: Creating new processes to execute commands

## 16. `wait`
* **Required Library**: `<sys/wait.h>`
* **Prototype**: `pid_t wait(int *wstatus);`
* **Parameters**: `wstatus` - status information about terminated child
* **Return Value**: PID of terminated child or -1 on error
* **Usage in Minishell**: Waiting for child processes to terminate

## 17. `waitpid`
* **Required Library**: `<sys/wait.h>`
* **Prototype**: `pid_t waitpid(pid_t pid, int *wstatus, int options);`
* **Parameters**: `pid` - PID of child, `wstatus` - status info, `options` - wait options
* **Return Value**: PID of terminated child or -1 on error
* **Usage in Minishell**: Waiting for specific child processes

## 18. `wait3`
* **Required Library**: `<sys/wait.h>`
* **Prototype**: `pid_t wait3(int *wstatus, int options, struct rusage *rusage);`
* **Parameters**: `wstatus` - status info, `options` - wait options, `rusage` - resource usage stats
* **Return Value**: PID of terminated child or -1 on error
* **Usage in Minishell**: Waiting for child processes with resource usage information

## 19. `wait4`
* **Required Library**: `<sys/wait.h>`
* **Prototype**: `pid_t wait4(pid_t pid, int *wstatus, int options, struct rusage *rusage);`
* **Parameters**: `pid` - PID of child, `wstatus` - status info, `options` - wait options, `rusage` - resource usage stats
* **Return Value**: PID of terminated child or -1 on error
* **Usage in Minishell**: Waiting for specific child processes with resource usage information

## 20. `signal`
* **Required Library**: `<signal.h>`
* **Prototype**: `void (*signal(int signum, void (*handler)(int)))(int);`
* **Parameters**: `signum` - signal number, `handler` - signal handler function
* **Return Value**: Previous signal handler or SIG_ERR on error
* **Usage in Minishell**: Setting up basic signal handlers

## 21. `sigaction`
* **Required Library**: `<signal.h>`
* **Prototype**: `int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact);`
* **Parameters**: `signum` - signal number, `act` - new action, `oldact` - previous action
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Advanced signal handling configuration

## 22. `sigemptyset`
* **Required Library**: `<signal.h>`
* **Prototype**: `int sigemptyset(sigset_t *set);`
* **Parameters**: `set` - signal set to initialize
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Initializing a signal set to empty

## 23. `sigaddset`
* **Required Library**: `<signal.h>`
* **Prototype**: `int sigaddset(sigset_t *set, int signum);`
* **Parameters**: `set` - signal set, `signum` - signal to add
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Adding signals to a signal set

## 24. `kill`
* **Required Library**: `<signal.h>`
* **Prototype**: `int kill(pid_t pid, int sig);`
* **Parameters**: `pid` - process ID, `sig` - signal to send
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Sending signals to processes

## 25. `exit`
* **Required Library**: `<stdlib.h>`
* **Prototype**: `void exit(int status);`
* **Parameters**: `status` - exit status code
* **Return Value**: Does not return
* **Usage in Minishell**: Terminating the shell or child processes

## 26. `getcwd`
* **Required Library**: `<unistd.h>`
* **Prototype**: `char *getcwd(char *buf, size_t size);`
* **Parameters**: `buf` - buffer to store path, `size` - buffer size
* **Return Value**: Pointer to buffer or NULL on error
* **Usage in Minishell**: Getting the current working directory

## 27. `chdir`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int chdir(const char *path);`
* **Parameters**: `path` - directory path to change to
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Changing the current working directory

## 28. `stat`
* **Required Library**: `<sys/stat.h>`
* **Prototype**: `int stat(const char *pathname, struct stat *statbuf);`
* **Parameters**: `pathname` - file path, `statbuf` - structure to fill
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Retrieving file information

## 29. `lstat`
* **Required Library**: `<sys/stat.h>`
* **Prototype**: `int lstat(const char *pathname, struct stat *statbuf);`
* **Parameters**: `pathname` - file path, `statbuf` - structure to fill
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Getting file information without following symlinks

## 30. `fstat`
* **Required Library**: `<sys/stat.h>`
* **Prototype**: `int fstat(int fd, struct stat *statbuf);`
* **Parameters**: `fd` - file descriptor, `statbuf` - structure to fill
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Getting information about open files

## 31. `unlink`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int unlink(const char *pathname);`
* **Parameters**: `pathname` - file to remove
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Removing temporary files

## 32. `execve`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int execve(const char *pathname, char *const argv[], char *const envp[]);`
* **Parameters**: `pathname` - program path, `argv` - argument array, `envp` - environment variables
* **Return Value**: Does not return on success, -1 on error
* **Usage in Minishell**: Executing commands

## 33. `dup`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int dup(int oldfd);`
* **Parameters**: `oldfd` - file descriptor to duplicate
* **Return Value**: New file descriptor or -1 on error
* **Usage in Minishell**: Duplicating file descriptors for redirection

## 34. `dup2`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int dup2(int oldfd, int newfd);`
* **Parameters**: `oldfd` - original file descriptor, `newfd` - target file descriptor
* **Return Value**: New file descriptor or -1 on error
* **Usage in Minishell**: Redirecting standard I/O

## 35. `pipe`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int pipe(int pipefd[2]);`
* **Parameters**: `pipefd` - array to store pipe file descriptors
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Creating pipes for command pipelines

## 36. `opendir`
* **Required Library**: `<dirent.h>`
* **Prototype**: `DIR *opendir(const char *name);`
* **Parameters**: `name` - directory path
* **Return Value**: Pointer to directory stream or NULL on error
* **Usage in Minishell**: Opening directories for reading

## 37. `readdir`
* **Required Library**: `<dirent.h>`
* **Prototype**: `struct dirent *readdir(DIR *dirp);`
* **Parameters**: `dirp` - directory stream
* **Return Value**: Pointer to directory entry or NULL on end/error
* **Usage in Minishell**: Reading directory contents (e.g., for command completion)

## 38. `closedir`
* **Required Library**: `<dirent.h>`
* **Prototype**: `int closedir(DIR *dirp);`
* **Parameters**: `dirp` - directory stream to close
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Closing directory streams

## 39. `strerror`
* **Required Library**: `<string.h>`
* **Prototype**: `char *strerror(int errnum);`
* **Parameters**: `errnum` - error number
* **Return Value**: Pointer to error message string
* **Usage in Minishell**: Getting error message strings

## 40. `perror`
* **Required Library**: `<stdio.h>`
* **Prototype**: `void perror(const char *s);`
* **Parameters**: `s` - prefix string
* **Return Value**: None
* **Usage in Minishell**: Printing error messages

## 41. `isatty`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int isatty(int fd);`
* **Parameters**: `fd` - file descriptor
* **Return Value**: 1 if terminal, 0 if not, -1 on error
* **Usage in Minishell**: Checking if I/O is connected to a terminal

## 42. `ttyname`
* **Required Library**: `<unistd.h>`
* **Prototype**: `char *ttyname(int fd);`
* **Parameters**: `fd` - file descriptor
* **Return Value**: Pointer to terminal name or NULL if not a terminal
* **Usage in Minishell**: Getting the name of the terminal

## 43. `ttyslot`
* **Required Library**: `<unistd.h>`
* **Prototype**: `int ttyslot(void);`
* **Parameters**: None
* **Return Value**: Slot number in utmp or -1 on error
* **Usage in Minishell**: Getting terminal information

## 44. `ioctl`
* **Required Library**: `<sys/ioctl.h>`
* **Prototype**: `int ioctl(int fd, unsigned long request, ...);`
* **Parameters**: `fd` - file descriptor, `request` - device-specific request, optional additional arguments
* **Return Value**: Device-specific, usually 0 on success, -1 on error
* **Usage in Minishell**: Terminal control operations

## 45. `getenv`
* **Required Library**: `<stdlib.h>`
* **Prototype**: `char *getenv(const char *name);`
* **Parameters**: `name` - environment variable name
* **Return Value**: Pointer to value or NULL if not found
* **Usage in Minishell**: Getting environment variables

## 46. `tcsetattr`
* **Required Library**: `<termios.h>`
* **Prototype**: `int tcsetattr(int fd, int optional_actions, const struct termios *termios_p);`
* **Parameters**: `fd` - file descriptor, `optional_actions` - when to apply changes, `termios_p` - terminal attributes
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Setting terminal attributes

## 47. `tcgetattr`
* **Required Library**: `<termios.h>`
* **Prototype**: `int tcgetattr(int fd, struct termios *termios_p);`
* **Parameters**: `fd` - file descriptor, `termios_p` - structure to fill
* **Return Value**: 0 on success, -1 on error
* **Usage in Minishell**: Getting terminal attributes

## 48. `tgetent`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `int tgetent(char *bp, const char *name);`
* **Parameters**: `bp` - buffer for storage, `name` - terminal name
* **Return Value**: 1 on success, 0 if not found, -1 on error
* **Usage in Minishell**: Terminal capability database initialization

## 49. `tgetflag`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `int tgetflag(char *id);`
* **Parameters**: `id` - capability name
* **Return Value**: 1 if capability exists, 0 otherwise
* **Usage in Minishell**: Checking terminal capabilities

## 50. `tgetnum`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `int tgetnum(char *id);`
* **Parameters**: `id` - capability name
* **Return Value**: Numeric value or -1 if not available
* **Usage in Minishell**: Getting numeric terminal capabilities

## 51. `tgetstr`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `char *tgetstr(char *id, char **area);`
* **Parameters**: `id` - capability name, `area` - buffer pointer
* **Return Value**: String value or NULL if not available
* **Usage in Minishell**: Getting string terminal capabilities

## 52. `tgoto`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `char *tgoto(const char *cap, int col, int row);`
* **Parameters**: `cap` - cursor motion capability, `col` - column, `row` - row
* **Return Value**: Formatted command string
* **Usage in Minishell**: Cursor movement in terminal

## 53. `tputs`
* **Required Library**: `<term.h>` or `<curses.h>` and `<term.h>`
* **Prototype**: `int tputs(const char *str, int affcnt, int (*putc)(int));`
* **Parameters**: `str` - capability string, `affcnt` - affected lines, `putc` - output function
* **Return Value**: OK or ERR
* **Usage in Minishell**: Outputting terminal capability strings