#include "../../../include/minishell.h"

void execute_command(t_minishell *mini, char **envp)
{
    int pipe_fds[2];
    pid_t pid;
    int status;
    pid_t wpid;
    int prev_fd = -1; // For input of current command

    for (int i = 0; i <= mini->pipex_count; i++)
    {
        // Create pipe for all but the last command
        if (i < mini->pipex_count)
        {
            if (pipe(pipe_fds) == -1)
            {
                perror("pipe");
                exit(EXIT_FAILURE);
            }
        }

        if (is_builtin(mini->cmd[i].argv[0]))
        {
            // Builtins run in parent process but handle input/output redirection
            if (prev_fd != -1)
            {
                // Duplicate input fd if necessary
                dup2(prev_fd, STDIN_FILENO);
                close(prev_fd);
            }

            if (i < mini->pipex_count)
            {
                close(pipe_fds[0]); // Close unused read end
                dup2(pipe_fds[1], STDOUT_FILENO);
                close(pipe_fds[1]);
            }

            execute_builtin(mini, i);
            prev_fd = (i < mini->pipex_count) ? pipe_fds[0] : -1;
        }
        else
        {
            // External commands executed in child process
            pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(EXIT_FAILURE);
            }
            else if (pid == 0)
            {
                // Child process

                if (prev_fd != -1)
                {
                    dup2(prev_fd, STDIN_FILENO);
                    close(prev_fd);
                }

                if (i < mini->pipex_count)
                {
                    close(pipe_fds[0]);
                    dup2(pipe_fds[1], STDOUT_FILENO);
                    close(pipe_fds[1]);
                }

                // Reset signals in child
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);

                char *path = resolve_cmd_path(mini->cmd[i].argv[0], envp);
                if (!path)
                {
                    fprintf(stderr, "minishell: command not found: %s\n", mini->cmd[i].argv[0]);
                    exit(127);
                }
                execve(path, mini->cmd[i].argv, envp); // Change this
                perror("execve");
                exit(EXIT_FAILURE);
            }
            else
            {
                // Parent process
                if (prev_fd != -1)
                    close(prev_fd);
                if (i < mini->pipex_count)
                    close(pipe_fds[1]);

                prev_fd = (i < mini->pipex_count) ? pipe_fds[0] : -1;
            }
        }
    }


    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            mini->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            mini->exit_status = 128 + WTERMSIG(status);
    }

}

int is_builtin(char *cmd)
{
    if (!cmd)
        return 0;
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd, "unset") ||
            !strcmp(cmd, "exit"));
}

void execute_builtin(t_minishell *minishell, int i)
{
    printf("BUILD IN COMMAND\n");
    char *cmd = minishell->cmd[i].argv[0];

    for (int j = 0; minishell->cmd[i].argv[j] != NULL; j++) {
        printf("TEST EXPORT: argv[%d]: %s\n", j, minishell->cmd[i].argv[j]);
    }
    if (strcmp(minishell->cmd[i].argv[0], "echo") == 0)
        call_echo(minishell->cmd[i].argv);
    else if (!strcmp(cmd, "pwd"))
        call_pwd();
    else if (!strcmp(cmd, "env"))
        call_env(minishell);
    else if (!strcmp(cmd, "export"))
        call_export(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "unset"))
        call_unset(minishell, minishell->cmd[i].argv);
    else if (!strcmp(cmd, "env")) {
        call_env(minishell);
        minishell->exit_status = 0; // success
    }
    else if (!strcmp(cmd, "unset")) {
        call_unset(minishell, minishell->cmd[i].argv);
        minishell->exit_status = 0; // or handle invalid identifiers and return 1
    }
    else if (!strcmp(cmd, "exit"))
        call_exit(minishell, minishell->cmd[i].argv);

    /*else if (!strcmp(cmd, "cd"))
        call_cd(minishell, envp);
    */
    //exit(0);
}

/*

dal-mahr@c1r6s5:~/Desktop/minihell$ jdfkl
jdfkl: command not found
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
127
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ls
include  libft	Makefile  minishell  Notes  obj  out.txt  readline.supp  srcs
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
0
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ jdfslk
jdfslk: command not found
dal-mahr@c1r6s5:~/Desktop/minihell$ exit
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
127
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ls jfdlk
ls: cannot access 'jfdlk': No such file or directory
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
2
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ^C
dal-mahr@c1r6s5:~/Desktop/minihell$ echo $?
130
dal-mahr@c1r6s5:~/Desktop/minihell$ cat
^\Quit (core dumped)
dal-mahr@c1r6s5:~/Desktop/minihell$ echo $?
131
dal-mahr@c1r6s5:~/Desktop/minihell$
*/
