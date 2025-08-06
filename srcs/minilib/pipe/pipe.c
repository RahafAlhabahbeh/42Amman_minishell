/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/27 13:38:33 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void safe_pipe(int pipe_fds[2])
{
    if (pipe(pipe_fds) == -1)
    {
        perror("pipe");
        exit(EXIT_FAILURE); // Or handle error as you want
    }
}

void count_pipe(t_minishell *minishell)
{
    int count = 0;
    t_token *cur = minishell->token;

    while (cur)
    {
        if (cur->type == PIPE)
            count++;
        cur = cur->next;
    }
    minishell->pipex_count = count;
}

void execute_piped_commands(t_minishell *minishell, char **envp)
{
    printf("PIPE EXE\n");
    int n = minishell->pipex_count + 1; // number of commands
    int pipefds[2 * (n - 1)];
    pid_t pid;
    int i;

    // Create pipes
    for (i = 0; i < n - 1; i++)
    {
        if (pipe(pipefds + i * 2) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < n; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            // Child process

            // Redirect input if not first command
            if (i != 0)
            {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Redirect output if not last command
            if (i != n - 1)
            {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1)
                {
                    perror("dup2");
                    exit(EXIT_FAILURE);
                }
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2 * (n - 1); j++)
                close(pipefds[j]);

            // Handle input/output redirection from minishell->cmd[i]
            if (minishell->cmd[i].input_file_name)
            {
                int fd_in = open(minishell->cmd[i].input_file_name, O_RDONLY);
                if (fd_in == -1)
                {
                    perror("open input file");
                    printf("Remove the exit 2\n");
                    exit(EXIT_FAILURE);
                }
                if (dup2(fd_in, STDIN_FILENO) == -1)
                {
                    perror("dup2 input");
                    exit(EXIT_FAILURE);
                }
                close(fd_in);
            }
            if (minishell->cmd[i].output_file_name)
            {
                int fd_out = open(minishell->cmd[i].output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd_out == -1)
                {
                    perror("open output file");
                    printf("Remove the exit 3\n");
                    exit(EXIT_FAILURE);
                }
                if (dup2(fd_out, STDOUT_FILENO) == -1)
                {
                    perror("dup2 output");
                    exit(EXIT_FAILURE);
                }
                close(fd_out);
            }

            // Execute command
            execve(minishell->cmd[i].argv[0], minishell->cmd[i].argv, envp);

            // If execve returns, there was an error
            perror("execve");
            exit(EXIT_FAILURE);
        }
    }

    // Parent closes all pipe fds
    for (i = 0; i < 2 * (n - 1); i++)
        close(pipefds[i]);

    // Parent waits for all children
    for (i = 0; i < n; i++)
        wait(NULL);
}
