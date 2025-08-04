// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   execute_command.c                                  :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/07/24 08:52:02 by dal-mahr          #+#    #+#             */
// /*   Updated: 2025/07/27 14:16:54 by dal-mahr         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

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
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
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
                char *path = resolve_cmd_path(minishell->cmd[i].argv[0], minishell->envp);
                int j = i;
                for (int h = 0; minishell->cmd[j].argv[h]; h++)
                {
                    printf("PATH: %s, CMD ARGV[%d][%d] = %s\n", path, j, h, minishell->cmd[j].argv[h]);
                }
                if (!path)
                {
                    fprintf(stderr, "%s: command not found\n", minishell->cmd[i].argv[0]);
                    exit(127);
                }

                if (execve(path, minishell->cmd[i].argv, envp) == -1)
                {
                    perror("execve");
                    exit(126); // permission denied or other exec error
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

    while (wait(NULL) > 0)
        ;
}

int is_builtin(char *cmd)
{
    /*return (!strcmp(cmd, "echo") || !strcmp(cmd, "cd") || !strcmp(cmd, "pwd") ||
            !strcmp(cmd, "env") || !strcmp(cmd, "export") || !strcmp(cmd, "unset"));*/
    // printf("BUILD IN COMMAND\n");
    if (!cmd)
        return 0;
    return (!strcmp(cmd, "echo") || !strcmp(cmd, "pwd") || !strcmp(cmd, "env"));
}

void execute_builtin(t_minishell *minishell, int i, char **envp)
{
    printf("BUILD IN COMMAND\n");
    char *cmd = minishell->cmd[i].argv[0];
    if (strcmp(minishell->cmd[i].argv[0], "echo") == 0)
        call_echo(minishell->cmd[i].argv);
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
