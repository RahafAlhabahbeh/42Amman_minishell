#include "../../../include/minishell.h"

void execute_command(t_minishell *minishell, char **envp)
{
    int i;
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;

    for (i = 0; i <= minishell->pipex_count; i++)
    {
        if (i < minishell->pipex_count && pipe(pipefd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == 0)
        {
            // Input redirection
            if (minishell->cmd[i].input_file_name)
                redirect_input(minishell->cmd[i].input_file_name);
            else if (prev_fd != -1)
                dup2(prev_fd, STDIN_FILENO);

            // Output redirection
            if (minishell->cmd[i].output_file_name)
                redirect_output(minishell->cmd[i].output_file_name);
            else if (i < minishell->pipex_count)
                dup2(pipefd[1], STDOUT_FILENO);

            // Close unused fds
            if (prev_fd != -1)
                close(prev_fd);
            if (i < minishell->pipex_count)
                close(pipefd[0]);
            if (!minishell->cmd[i].argv || !minishell->cmd[i].argv[0])
		{
		    fprintf(stderr, "No command to execute\n");
		    return;
		}

            // Execute command
            if (is_builtin(minishell->cmd[0].argv[0]) && minishell->pipex_count == 0)
		{
		    execute_builtin(minishell, 0, envp);
		    continue;
		}            
            else
            {
                char *path = my_getenv(minishell->cmd[i].argv[0], minishell->envp);
                if (!path || execve(path, minishell->cmd[i].argv, envp) == -1)
                {
                    perror("execve");
                    exit(EXIT_FAILURE);
                }
                
            }
        }
        else if (pid < 0)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Parent process
        if (prev_fd != -1)
            close(prev_fd);
        if (i < minishell->pipex_count)
        {
            close(pipefd[1]);
            prev_fd = pipefd[0];
        }
    }

    while (wait(NULL) > 0);
}

int is_builtin(char *cmd)
{
    /*return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd, "unset"));*/
    printf("BUILD IN COMMAND\n");
    if (!cmd) return 0;
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env"));
}

void execute_builtin(t_minishell *minishell, int i, char **envp)
{
    char *cmd = minishell->cmd[i].argv[0];
    if (strcmp(minishell->cmd[i].argv[0], "echo") == 0)
    {
    	call_echo(minishell->cmd[i].argv);
}

    //if (!strcmp(cmd, "echo"))
        //call_echo(minishell, minishell->cmd[i].argv[1] && !strcmp(minishell->cmd[i].argv[1], "-n"));
    else if (!strcmp(cmd, "pwd"))
        call_pwd();
    else if (!strcmp(cmd, "env"))
        call_env(minishell, envp);
    /*else if (!strcmp(cmd, "cd"))
        call_cd(minishell, envp);
    else if (!strcmp(cmd, "export"))
        call_export(envp, minishell->cmd[i].argv[1]);
    else if (!strcmp(cmd, "unset"))
        call_unset(envp, minishell->cmd[i].argv[1]);
    */
    exit(0);
}

