# UNIX Functions with Practical Examples for Minishell

## 1. `readline`
```c
#include <readline/readline.h>
#include <stdio.h>

int main() {
    char *input;
    
    // Display prompt and read user input
    input = readline("minishell$ ");
    
    if (input) {
        printf("You entered: %s\n", input);
        free(input);  // readline allocates memory that must be freed
    }
    
    return 0;
}
```

## 2. `rl_clear_history`
```c
#include <readline/readline.h>
#include <readline/history.h>

void clear_cmd_history() {
    rl_clear_history();  // Clears the entire command history
    printf("Command history cleared\n");
}
```

## 3. `rl_on_new_line`
```c
#include <readline/readline.h>
#include <signal.h>

void handle_sigint(int sig) {
    write(1, "\n", 1);  // Print a newline
    rl_on_new_line();    // Tell readline we're on a new line
    rl_replace_line("", 0);  // Clear the current line
    rl_redisplay();      // Redisplay the prompt
}
```

## 4. `rl_replace_line`
```c
#include <readline/readline.h>

void clear_current_line() {
    rl_replace_line("", 1);  // Replace current line with empty string and clear undo history
    rl_redisplay();          // Update the display
}
```

## 5. `rl_redisplay`
```c
#include <readline/readline.h>

void reset_prompt() {
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();  // Refresh the display to show the updated line
}
```

## 6. `add_history`
```c
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    char *input;
    
    while (1) {
        input = readline("minishell$ ");
        if (!input)
            break;
            
        // Only add non-empty commands to history
        if (input[0] != '\0') {
            add_history(input);  // Add command to history for up/down arrow recall
        }
        
        // Process the command...
        
        free(input);
    }
    
    return 0;
}
```

## 7. `printf`
```c
#include <stdio.h>

void display_welcome() {
    printf("Welcome to %s version %1.1f\n", "Minishell", 1.0);
    printf("Type 'exit' to quit\n");
}
```

## 8. `malloc`
```c
#include <stdlib.h>
#include <string.h>

char *duplicate_string(const char *src) {
    char *dup;
    
    if (!src)
        return NULL;
        
    // Allocate memory for the duplicate string
    dup = (char *)malloc(strlen(src) + 1);
    if (!dup)
        return NULL;
        
    strcpy(dup, src);
    return dup;
}
```

## 9. `free`
```c
#include <stdlib.h>

void clean_command_table(char **cmd_table) {
    int i;
    
    if (!cmd_table)
        return;
        
    // Free each string in the array
    for (i = 0; cmd_table[i]; i++) {
        free(cmd_table[i]);
    }
    
    // Free the array itself
    free(cmd_table);
}
```

## 10. `write`
```c
#include <unistd.h>

void display_error(const char *msg) {
    // Write error message to stderr (file descriptor 2)
    write(2, "Error: ", 7);
    write(2, msg, strlen(msg));
    write(2, "\n", 1);
}
```

## 11. `access`
```c
#include <unistd.h>
#include <stdio.h>

int is_executable(const char *path) {
    // Check if file exists and is executable
    if (access(path, F_OK | X_OK) == 0) {
        return 1;  // File exists and is executable
    }
    return 0;      // File doesn't exist or isn't executable
}
```

## 12. `open`
```c
#include <fcntl.h>
#include <unistd.h>

int redirect_output(const char *filename) {
    int fd;
    
    // Open file for writing, create if doesn't exist, truncate if exists
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("open");
        return -1;
    }
    
    // Replace stdout with our file
    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return -1;
    }
    
    close(fd);  // Close original fd as it's duplicated now
    return 0;
}
```

## 13. `read`
```c
#include <unistd.h>
#include <stdio.h>

void read_from_pipe(int pipe_fd) {
    char buffer[1024];
    ssize_t bytes_read;
    
    // Read data from pipe into buffer
    bytes_read = read(pipe_fd, buffer, sizeof(buffer) - 1);
    if (bytes_read > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the string
        printf("Read from pipe: %s\n", buffer);
    } else if (bytes_read == 0) {
        printf("Pipe closed (EOF)\n");
    } else {
        perror("read");
    }
}
```

## 14. `close`
```c
#include <unistd.h>

void cleanup_fds(int *fd_array, int count) {
    int i;
    
    // Close all file descriptors in the array
    for (i = 0; i < count; i++) {
        if (fd_array[i] >= 0) {
            close(fd_array[i]);
            fd_array[i] = -1;  // Mark as closed
        }
    }
}
```

## 15. `fork`
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void execute_external_command(const char *cmd) {
    pid_t pid;
    
    pid = fork();  // Create a new process
    if (pid < 0) {
        // Error
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        execlp(cmd, cmd, NULL);
        // If we get here, execlp failed
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        printf("Spawned child process with PID: %d\n", pid);
        // Parent would typically wait for child here
    }
}
```

## 16. `wait`
```c
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int execute_and_wait(const char *cmd) {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child
        execlp(cmd, cmd, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent
        wait(&status);  // Wait for ANY child process to terminate
        
        if (WIFEXITED(status)) {
            printf("Child exited with status: %d\n", WEXITSTATUS(status));
            return WEXITSTATUS(status);
        }
    }
    return -1;
}
```

## 17. `waitpid`
```c
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int execute_and_wait_specific(const char *cmd) {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    } else if (pid == 0) {
        // Child
        execlp(cmd, cmd, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent
        // Wait specifically for the child we just created
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Child %d exited with status: %d\n", pid, WEXITSTATUS(status));
            return WEXITSTATUS(status);
        }
    }
    return -1;
}
```

## 18. `wait3`
```c
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

void execute_with_resource_stats(const char *cmd) {
    pid_t pid, wpid;
    int status;
    struct rusage usage;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child
        execlp(cmd, cmd, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent
        // Wait for any child and get resource usage info
        wpid = wait3(&status, 0, &usage);
        
        if (wpid == pid) {
            printf("Command used %ld.%06ld CPU seconds\n", 
                   usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
        }
    }
}
```

## 19. `wait4`
```c
#include <sys/wait.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

void execute_with_specific_resource_stats(const char *cmd) {
    pid_t pid;
    int status;
    struct rusage usage;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child
        execlp(cmd, cmd, NULL);
        perror("execlp");
        exit(EXIT_FAILURE);
    } else {
        // Parent
        // Wait specifically for our child and get resource usage info
        wait4(pid, &status, 0, &usage);
        
        printf("Command used %ld.%06ld CPU seconds\n", 
               usage.ru_utime.tv_sec, usage.ru_utime.tv_usec);
    }
}
```

## 20. `signal`
```c
#include <signal.h>
#include <stdio.h>

void handle_signal(int sig) {
    printf("\nReceived signal %d\n", sig);
}

void setup_signal_handlers() {
    // Set up handler for Ctrl+C (SIGINT)
    signal(SIGINT, handle_signal);
    
    // Ignore Ctrl+Z (SIGTSTP)
    signal(SIGTSTP, SIG_IGN);
}
```

## 21. `sigaction`
```c
#include <signal.h>
#include <stdio.h>
#include <string.h>

void sigint_handler(int sig) {
    printf("\nCaught SIGINT (Ctrl+C)\n");
}

void setup_advanced_signal_handlers() {
    struct sigaction sa;
    
    // Initialize the sigaction struct
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sigint_handler;
    
    // Block all signals during handler execution
    sigfillset(&sa.sa_mask);
    
    // Set flags
    sa.sa_flags = SA_RESTART;  // Restart interrupted system calls
    
    // Set up the handler for SIGINT
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
    }
}
```

## 22. `sigemptyset`
```c
#include <signal.h>
#include <stdio.h>

void initialize_signal_set() {
    sigset_t set;
    
    // Initialize an empty signal set
    if (sigemptyset(&set) == -1) {
        perror("sigemptyset");
        return;
    }
    
    printf("Signal set initialized\n");
    // Now ready to add signals to the set
}
```

## 23. `sigaddset`
```c
#include <signal.h>
#include <stdio.h>

void create_custom_signal_set() {
    sigset_t set;
    
    // Initialize empty set
    sigemptyset(&set);
    
    // Add specific signals to the set
    sigaddset(&set, SIGINT);   // Ctrl+C
    sigaddset(&set, SIGTERM);  // Termination signal
    sigaddset(&set, SIGQUIT);  // Quit signal
    
    printf("Custom signal set created\n");
    // This set could be used with sigprocmask, sigaction, etc.
}
```

## 24. `kill`
```c
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void terminate_child_process(pid_t child_pid) {
    // First try a gentle termination
    if (kill(child_pid, SIGTERM) == -1) {
        perror("kill (SIGTERM)");
        return;
    }
    
    // Wait a bit for child to terminate
    sleep(1);
    
    // If process still exists, force kill
    if (kill(child_pid, 0) == 0) {  // Check if process exists
        printf("Child didn't terminate, sending SIGKILL\n");
        if (kill(child_pid, SIGKILL) == -1) {
            perror("kill (SIGKILL)");
        }
    } else {
        printf("Child process terminated successfully\n");
    }
}
```

## 25. `exit`
```c
#include <stdlib.h>
#include <stdio.h>

void handle_exit_command(const char *arg) {
    int status = 0;
    
    // If argument provided, use it as exit status
    if (arg) {
        status = atoi(arg);
    }
    
    printf("Exiting shell with status %d\n", status);
    exit(status);  // Terminate the process with given status
}
```

## 26. `getcwd`
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void print_current_directory() {
    char *cwd;
    char buffer[PATH_MAX];
    
    // Get current working directory
    cwd = getcwd(buffer, PATH_MAX);
    if (cwd) {
        printf("Current directory: %s\n", cwd);
    } else {
        perror("getcwd");
    }
}
```

## 27. `chdir`
```c
#include <unistd.h>
#include <stdio.h>

int change_directory(const char *path) {
    // Change to the specified directory
    if (chdir(path) == -1) {
        perror("chdir");
        return -1;
    }
    
    // Print new current directory
    printf("Changed to directory: ");
    print_current_directory();  // Using function from previous example
    return 0;
}
```

## 28. `stat`
```c
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>

void display_file_info(const char *path) {
    struct stat file_stat;
    
    // Get file information
    if (stat(path, &file_stat) == -1) {
        perror("stat");
        return;
    }
    
    printf("File: %s\n", path);
    printf("Size: %ld bytes\n", file_stat.st_size);
    printf("Permissions: %o\n", file_stat.st_mode & 0777);
    printf("Last modified: %s", ctime(&file_stat.st_mtime));
    
    // Check if it's a directory
    if (S_ISDIR(file_stat.st_mode)) {
        printf("Type: Directory\n");
    } else if (S_ISREG(file_stat.st_mode)) {
        printf("Type: Regular file\n");
    }
}
```

## 29. `lstat`
```c
#include <sys/stat.h>
#include <stdio.h>

void check_if_symlink(const char *path) {
    struct stat file_stat;
    
    // Get file information without following symlinks
    if (lstat(path, &file_stat) == -1) {
        perror("lstat");
        return;
    }
    
    // Check if it's a symbolic link
    if (S_ISLNK(file_stat.st_mode)) {
        char link_target[PATH_MAX];
        ssize_t len;
        
        printf("%s is a symbolic link\n", path);
        
        // Get the target of the symlink
        len = readlink(path, link_target, PATH_MAX - 1);
        if (len != -1) {
            link_target[len] = '\0';
            printf("Link target: %s\n", link_target);
        }
    } else {
        printf("%s is not a symbolic link\n", path);
    }
}
```

## 30. `fstat`
```c
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

void get_open_file_info(const char *path) {
    int fd;
    struct stat file_stat;
    
    // Open the file
    fd = open(path, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Get information about the open file
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        close(fd);
        return;
    }
    
    printf("File size: %ld bytes\n", file_stat.st_size);
    printf("Inode number: %lu\n", file_stat.st_ino);
    
    close(fd);
}
```

## 31. `unlink`
```c
#include <unistd.h>
#include <stdio.h>

void remove_temp_file(const char *path) {
    // Remove the file from the filesystem
    if (unlink(path) == -1) {
        perror("unlink");
        return;
    }
    
    printf("Temporary file '%s' has been removed\n", path);
}
```

## 32. `execve`
```c
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

void execute_command(const char *path, char *const argv[], char *const envp[]) {
    pid_t pid;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        execve(path, argv, envp);
        
        // If we reach here, execve failed
        perror("execve");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
        
        if (WIFEXITED(status)) {
            printf("Command exited with status %d\n", WEXITSTATUS(status));
        }
    }
}

// Example usage:
// char *args[] = {"/bin/ls", "-la", NULL};
// char *env[] = {"PATH=/bin:/usr/bin", NULL};
// execute_command("/bin/ls", args, env);
```

## 33. `dup`
```c
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

void redirect_to_file(const char *filename) {
    int fd, saved_stdout;
    
    // Open the file
    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return;
    }
    
    // Save the current stdout
    saved_stdout = dup(STDOUT_FILENO);
    if (saved_stdout == -1) {
        perror("dup");
        close(fd);
        return;
    }
    
    // Redirect stdout to our file
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(fd);
        close(saved_stdout);
        return;
    }
    
    // Now printf will go to the file
    printf("This message goes to the file\n");
    
    // Restore stdout
    dup2(saved_stdout, STDOUT_FILENO);
    
    // This will go to terminal again
    printf("Back to terminal output\n");
    
    // Clean up
    close(fd);
    close(saved_stdout);
}
```

## 34. `dup2`
```c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

void redirect_input_from_file(const char *filename) {
    int fd, saved_stdin;
    
    // Save original stdin
    saved_stdin = dup(STDIN_FILENO);
    if (saved_stdin == -1) {
        perror("dup");
        return;
    }
    
    // Open file for reading
    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        close(saved_stdin);
        return;
    }
    
    // Replace stdin with our file
    if (dup2(fd, STDIN_FILENO) == -1) {
        perror("dup2");
        close(fd);
        close(saved_stdin);
        return;
    }
    
    // Now any read from stdin will come from the file
    char buffer[256];
    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        printf("Read from file: %s", buffer);
    }
    
    // Restore stdin
    dup2(saved_stdin, STDIN_FILENO);
    
    // Clean up
    close(fd);
    close(saved_stdin);
}
```

## 35. `pipe`
```c
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void create_pipe_between_processes() {
    int pipefd[2];
    pid_t pid;
    char buffer[100];
    
    // Create a pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return;
    }
    
    pid = fork();
    if (pid == -1) {
        perror("fork");
        close(pipefd[0]);
        close(pipefd[1]);
        return;
    } else if (pid == 0) {
        // Child process (reader)
        close(pipefd[1]);  // Close unused write end
        
        // Read from pipe
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            printf("Child received: %s\n", buffer);
        }
        
        close(pipefd[0]);
        exit(EXIT_SUCCESS);
    } else {
        // Parent process (writer)
        close(pipefd[0]);  // Close unused read end
        
        // Write to pipe
        const char *message = "Hello from parent!";
        write(pipefd[1], message, strlen(message));
        
        close(pipefd[1]);
        wait(NULL);  // Wait for child to complete
    }
}
```

## 36. `opendir`
```c
#include <dirent.h>
#include <stdio.h>

void list_directory_contents(const char *path) {
    DIR *dir;
    struct dirent *entry;
    
    // Open directory
    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }
    
    printf("Contents of directory %s:\n", path);
    
    // Read directory entries
    while ((entry = readdir(dir)) != NULL) {
        printf("  %s\n", entry->d_name);
    }
    
    closedir(dir);
}
```

## 37. `readdir`
```c
#include <dirent.h>
#include <stdio.h>
#include <string.h>

void find_files_by_extension(const char *path, const char *ext) {
    DIR *dir;
    struct dirent *entry;
    size_t ext_len = strlen(ext);
    
    // Open directory
    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return;
    }
    
    printf("Files with extension '%s' in %s:\n", ext, path);
    
    // Read each entry
    while ((entry = readdir(dir)) != NULL) {
        size_t name_len = strlen(entry->d_name);
        
        // Check if file ends with the extension
        if (name_len > ext_len && 
            strcmp(entry->d_name + name_len - ext_len, ext) == 0) {
            printf("  %s\n", entry->d_name);
        }
    }
    
    closedir(dir);
}
```

## 38. `closedir`
```c
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>

int count_directory_entries(const char *path) {
    DIR *dir;
    struct dirent *entry;
    int count = 0;
    
    // Open directory
    dir = opendir(path);
    if (!dir) {
        perror("opendir");
        return -1;
    }
    
    // Count entries (including . and ..)
    while ((entry = readdir(dir)) != NULL) {
        count++;
    }
    
    // Close the directory when finished
    if (closedir(dir) == -1) {
        perror("closedir");
        return -1;
    }
    
    return count;
}
```

## 39. `strerror`
```c
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

void try_open_file(const char *path) {
    int fd = open(path, O_RDONLY);
    
    if (fd == -1) {
        // Get the string representation of the error
        printf("Failed to open '%s': %s\n", path, strerror(errno));
    } else {
        printf("Successfully opened '%s'\n", path);
        close(fd);
    }
}
```

## 40. `perror`
```c
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

void try_create_file(const char *path) {
    int fd = open(path, O_WRONLY | O_CREAT | O_EXCL, 0644);
    
    if (fd == -1) {
        // Print error message with the given prefix
        perror("File creation error");
    } else {
        printf("File created successfully\n");
        close(fd);
    }
}
```

## 41. `isatty`
```c
#include <unistd.h>
#include <stdio.h>

void check_if_terminal() {
    if (isatty(STDIN_FILENO)) {
        printf("Standard input is connected to a terminal\n");
    } else {
        printf("Standard input is NOT connected to a terminal\n");
    }
    
    if (isatty(STDOUT_FILENO)) {
        printf("Standard output is connected to a terminal\n");
    } else {
        printf("Standard output is NOT connected to a terminal\n");
    }
}
```

## 42. `ttyname`
```c
#include <unistd.h>
#include <stdio.h>

void print_terminal_name() {
    char *term_name;
    
    if (isatty(STDOUT_FILENO)) {
        term_name = ttyname(STDOUT_FILENO);
        if (term_name) {
            printf("Terminal name: %s\n", term_name);
        } else {
            perror("ttyname");
        }
    } else {
        printf("Standard output is not connected to a terminal\n");
    }
}
```

## 43. `ttyslot`
```c
#include <unistd.h>
#include <stdio.h>

void print_tty_slot() {
    int slot = ttyslot();
    
    if (slot != -1) {
        printf("TTY slot: %d\n", slot);
    } else {
        perror("ttyslot");
    }
}
```

## 44. `ioctl`
```c
#include <sys/ioctl.h>
#include <unistd.h>
#include <stdio.h>

void get_terminal_size() {
    struct winsize ws;
    
    // Get terminal window size
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
        perror("ioctl");
        return;
    }
    
    printf("Terminal size: %d rows x %d columns\n", ws.ws_row, ws.ws_col);
}
```

## 45. `getenv`
```c
#include <stdlib.h>
#include <stdio.h>

void print_environment_variable(const char *name) {
    char *value;
    
    // Get the value of the environment variable
    value = getenv(name);
    
    if (value) {
        printf("%s=%s\n", name, value);
    } else {
        printf("Environment variable %s is not set\n", name);  
    }
}
```

## 46. `tcsetattr`
```c
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

void disable_terminal_echo() {
    struct termios term;
    
    // Get current terminal attributes
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return;
    }
    
    // Disable echo
    term.c_lflag &= ~ECHO;
    
    // Set the modified attributes
    if (tcsetattr(STDIN_FILENO, TCSANOW, &term) == -1) {
        perror("tcsetattr");
        return;
    }
    
    printf("Terminal echo is now disabled\n");
}
```

## 47. `tcgetattr`
```c
#include <termios.h>
#include <unistd.h>
#include <stdio.h>

void save_terminal_settings() {
    struct termios term;
    
    // Get the current terminal attributes
    if (tcgetattr(STDIN_FILENO, &term) == -1) {
        perror("tcgetattr");
        return;
    }
    
    // Save original settings for later restoration
    // In a real shell, you'd store this in a global variable
    printf("Terminal settings saved\n");
    
    // Example of checking if canonical mode is enabled
    if (term.c_lflag & ICANON) {
        printf("Terminal is in canonical (line) mode\n");
    } else {
        printf("Terminal is in non-canonical (raw) mode\n");
    }
}
```

## 48. `tgetent`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>

void initialize_terminal_database() {
    char term_buffer[2048];
    char *term_type;
    
    // Get terminal type from environment
    term_type = getenv("TERM");
    if (!term_type) {
        fprintf(stderr, "TERM environment variable not set\n");
        return;
    }
    
    // Load terminal capabilities
    int result = tgetent(term_buffer, term_type);
    if (result < 0) {
        fprintf(stderr, "Could not access the terminal database\n");
        return;
    } else if (result == 0) {
        fprintf(stderr, "Terminal type '%s' is not defined\n", term_type);
        return;
    }
    
    printf("Terminal type '%s' capabilities loaded\n", term_type);
}
```

## 49. `tgetflag`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>

void check_terminal_capabilities() {
    char term_buffer[2048];
    
    // Load terminal capabilities
    if (tgetent(term_buffer, getenv("TERM")) <= 0) {
        fprintf(stderr, "Could not load terminal capabilities\n");
        return;
    }
    
    // Check if terminal can move the cursor
    if (tgetflag("cu")) {
        printf("Terminal supports cursor movement\n");
    } else {
        printf("Terminal does not support cursor movement\n");
    }
    
    // Check if terminal supports automatic margins (wrapping at edge)
    if (tgetflag("am")) {
        printf("Terminal has automatic margins\n");
    } else {
        printf("Terminal does not have automatic margins\n");
    }
}
```

## 50. `tgetnum`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>

void display_terminal_dimensions() {
    char term_buffer[2048];
    
    // Load terminal capabilities
    if (tgetent(term_buffer, getenv("TERM")) <= 0) {
        fprintf(stderr, "Could not load terminal capabilities\n");
        return;
    }
    
    // Get number of columns
    int cols = tgetnum("co");
    if (cols != -1) {
        printf("Terminal has %d columns\n", cols);
    } else {
        printf("Could not determine number of columns\n");
    }
    
    // Get number of lines
    int lines = tgetnum("li");
    if (lines != -1) {
        printf("Terminal has %d lines\n", lines);
    } else {
        printf("Could not determine number of lines\n");
    }
}
```

## 51. `tgetstr`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

void get_terminal_string_capabilities() {
    char term_buffer[2048];
    char buffer[1024];
    char *bp = buffer;
    
    // Load terminal capabilities
    if (tgetent(term_buffer, getenv("TERM")) <= 0) {
        fprintf(stderr, "Could not load terminal capabilities\n");
        return;
    }
    
    // Get clear screen capability
    char *cl = tgetstr("cl", &bp);
    if (cl) {
        printf("Terminal has capability to clear screen\n");
    } else {
        printf("Terminal cannot clear screen\n");
    }
    
    // Get cursor movement capability
    char *cm = tgetstr("cm", &bp);
    if (cm) {
        printf("Terminal can move cursor to arbitrary position\n");
    } else {
        printf("Terminal cannot move cursor to arbitrary position\n");
    }
}
```

## 52. `tgoto`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

void move_cursor_to_position(int col, int row) {
    char term_buffer[2048];
    char buffer[1024];
    char *bp = buffer;
    
    // Load terminal capabilities
    if (tgetent(term_buffer, getenv("TERM")) <= 0) {
        fprintf(stderr, "Could not load terminal capabilities\n");
        return;
    }
    
    // Get cursor movement capability
    char *cm = tgetstr("cm", &bp);
    if (!cm) {
        fprintf(stderr, "Terminal cannot move cursor\n");
        return;
    }
    
    // Format the cursor movement string for the given position
    char *cursor_command = tgoto(cm, col, row);
    
    // Apply the cursor movement
    tputs(cursor_command, 1, putchar);
    
    // Now we're at the requested position, print something
    printf("X");  // Mark the position
}
```

## 53. `tputs`
```c
#include <curses.h>
#include <term.h>
#include <stdio.h>
#include <string.h>

void clear_terminal_screen() {
    char term_buffer[2048];
    char buffer[1024];
    char *bp = buffer;
    
    // Load terminal capabilities
    if (tgetent(term_buffer, getenv("TERM")) <= 0) {
        fprintf(stderr, "Could not load terminal capabilities\n");
        return;
    }
    
    // Get clear screen capability
    char *cl = tgetstr("cl", &bp);
    if (!cl) {
        fprintf(stderr, "Terminal cannot clear screen\n");
        return;
    }
    
    // Apply the clear screen capability
    tputs(cl, 1, putchar);
    
    printf("Screen has been cleared\n");
}
```

## Complete Minishell Example

Here's a simple but comprehensive example that pulls together many of these functions to create a basic shell:

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_ARGS 64
#define MAX_PATH 1024

// Global variables
int running = 1;

// Signal handler for Ctrl+C
void handle_sigint(int sig) {
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

// Set up signal handlers
void setup_signals() {
    struct sigaction sa;
    
    // Initialize the sigaction struct
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_sigint;
    sa.sa_flags = SA_RESTART;
    
    // Set up SIGINT handler (Ctrl+C)
    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    
    // Ignore SIGQUIT (Ctrl+\)
    signal(SIGQUIT, SIG_IGN);
    
    // Ignore SIGTSTP (Ctrl+Z)
    signal(SIGTSTP, SIG_IGN);
}

// Parse the command into an array of arguments
int parse_command(char *cmd, char *args[]) {
    int count = 0;
    char *token;
    
    token = strtok(cmd, " \t\n");
    while (token != NULL && count < MAX_ARGS - 1) {
        args[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[count] = NULL;
    
    return count;
}

// Check if command is a built-in command
int execute_builtin(char *args[]) {
    // Handle 'exit' command
    if (strcmp(args[0], "exit") == 0) {
        printf("Goodbye!\n");
        running = 0;
        return 1;
    }
    
    // Handle 'cd' command
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            // Default to home directory
            char *home = getenv("HOME");
            if (home) {
                chdir(home);
            } else {
                fprintf(stderr, "cd: HOME not set\n");
            }
        } else if (chdir(args[1]) != 0) {
            perror("cd");
        }
        return 1;
    }
    
    // Handle 'pwd' command
    if (strcmp(args[0], "pwd") == 0) {
        char cwd[MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        } else {
            perror("pwd");
        }
        return 1;
    }
    
    return 0; // Not a builtin command
}

// Execute an external command
void execute_external(char *args[]) {
    pid_t pid;
    int status;
    
    pid = fork();
    if (pid < 0) {
        perror("fork");
        return;
    } else if (pid == 0) {
        // Child process
        
        // Execute the command
        execvp(args[0], args);
        
        // If we get here, execvp failed
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        // Parent process
        waitpid(pid, &status, 0);
    }
}

// Main shell loop
int main() {
    char *input;
    char *args[MAX_ARGS];
    
    // Set up signal handling
    setup_signals();
    
    // Welcome message
    printf("Welcome to Minishell!\n");
    
    // Main loop
    while (running) {
        // Show prompt and read input
        char cwd[MAX_PATH];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            char prompt[MAX_PATH + 4];
            sprintf(prompt, "%s> ", cwd);
            input = readline(prompt);
        } else {
            input = readline("minishell> ");
        }
        
        // Check for EOF (Ctrl+D)
        if (!input) {
            printf("\nGoodbye!\n");
            break;
        }
        
        // Skip empty lines
        if (input[0] == '\0') {
            free(input);
            continue;
        }
        
        // Add command to history
        add_history(input);
        
        // Parse the command
        if (parse_command(input, args) > 0) {
            // Try to execute as builtin
            if (!execute_builtin(args)) {
                // Not a builtin, execute as external command
                execute_external(args);
            }
        }
        
        // Free the allocated memory
        free(input);
    }
    
    // Clean up readline history
    rl_clear_history();
    
    return 0;
}
```

This example demonstrates:
1. Command line input using readline
2. Command history management
3. Signal handling for Ctrl+C, Ctrl+\ and Ctrl+Z
4. Parsing commands
5. Built-in commands (exit, cd, pwd)
6. Executing external commands
7. Memory management
8. Current directory display in prompt

This covers many of the system calls and functions that are essential for building a shell. For a complete minishell implementation, you would also need to add features like:
- Input/output redirection (>, >>, <)
- Pipes (|)
- Environment variable handling
- Wildcards and pattern matching
- Command autocompletion
- More built-in commands