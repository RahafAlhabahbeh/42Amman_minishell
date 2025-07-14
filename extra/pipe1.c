#include "../libft/libft.h"
#include <sys/wait.h>
#include <string.h>

typedef struct {
    char **args;
    int count;
    int capacity;
} ArgvArray;

void check_alloc(void *ptr) {
    if (!ptr) {
        perror("malloc/realloc failed");
        exit(EXIT_FAILURE);
    }
}

void init_argv_array(ArgvArray *arr) {
    arr->capacity = 10;
    arr->count = 0;
    arr->args = malloc(sizeof(char*) * arr->capacity);
    check_alloc(arr->args);
}

void add_arg(ArgvArray *arr, char *arg) {
    if (arr->count >= arr->capacity) {
        arr->capacity *= 2;
        arr->args = realloc(arr->args, sizeof(char*) * arr->capacity);
        check_alloc(arr->args);
    }
    arr->args[arr->count++] = arg;
}

void free_argv_array(ArgvArray *arr) {
    free(arr->args);
}

ArgvArray *parse_commands(int argc, char *argv[], int *cmd_count) {
    int cap = 10;
    ArgvArray *cmds = malloc(sizeof(ArgvArray) * cap);
    check_alloc(cmds);
    *cmd_count = 0;
    init_argv_array(&cmds[*cmd_count]);

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "|") == 0) {
            add_arg(&cmds[*cmd_count], NULL);
            if (++(*cmd_count) >= cap) {
                cap *= 2;
                cmds = realloc(cmds, sizeof(ArgvArray) * cap);
                check_alloc(cmds);
            }
            init_argv_array(&cmds[*cmd_count]);
        } else {
            add_arg(&cmds[*cmd_count], argv[i]);
        }
    }
    add_arg(&cmds[*cmd_count], NULL);
    (*cmd_count)++;
    return cmds;
}

int *create_pipes(int cmd_count) {
    if (cmd_count < 2) return NULL;
    int *fds = malloc(sizeof(int) * 2 * (cmd_count - 1));
    check_alloc(fds);
    for (int i = 0; i < cmd_count - 1; i++) {
        if (pipe(fds + i * 2) < 0) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }
    return fds;
}

void exec_command(ArgvArray *cmds, int *pipefd, int i, int cmd_count, char **envp) {
    if (i != 0) dup2(pipefd[(i - 1) * 2], STDIN_FILENO);
    if (i != cmd_count - 1) dup2(pipefd[i * 2 + 1], STDOUT_FILENO);

    if (pipefd)
        for (int j = 0; j < 2 * (cmd_count - 1); j++) close(pipefd[j]);

    execvp(cmds[i].args[0], cmds[i].args);
    perror("execvp failed");
    exit(EXIT_FAILURE);
}

void fork_all(ArgvArray *cmds, int *pipefd, int cmd_count, char **envp) {
    for (int i = 0; i < cmd_count; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if (pid == 0)
            exec_command(cmds, pipefd, i, cmd_count, envp);
    }
}

void cleanup(int *pipefd, ArgvArray *cmds, int cmd_count) {
    if (pipefd) {
        for (int i = 0; i < 2 * (cmd_count - 1); i++)
            close(pipefd[i]);
        free(pipefd);
    }
    for (int i = 0; i < cmd_count; i++) {
        wait(NULL);
        free_argv_array(&cmds[i]);
    }
    free(cmds);
}

int main(int argc, char *argv[], char **envp) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s cmd1 [args...] '|' cmd2 [args...] ...\n", argv[0]);
        return 1;
    }

    int cmd_count;
    ArgvArray *commands = parse_commands(argc, argv, &cmd_count);
    int *pipefd = create_pipes(cmd_count);
    fork_all(commands, pipefd, cmd_count, envp);
    cleanup(pipefd, commands, cmd_count);

    return 0;
}

