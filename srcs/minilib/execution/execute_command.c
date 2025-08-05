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


int is_str_in_set(char *s, char *set[])
{
    int i = 0;
    while (set[i])
    {
        if (ft_strcmp(s, set[i]) == 0)
            return 1;
        i++;
    }
    return 0;
}

int is_redirection_present(t_cmd *cmd)
{
    if (cmd->in_type == REDIR_IN || cmd->in_type == HERE_DOC)
        return 1;
    if (cmd->out_type == REDIR_OUT || cmd->out_type == REDIR_APPEND)
        return 1;
    return 0;
}


int should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes)
{
    int has_pipe = (index > 0) || (index < total_pipes);
    int has_redir = is_redirection_present(cmd);

    if (is_str_in_set(cmd->argv[0], (char *[]){"export", "unset", "cd", "exit", NULL}))
        return (!has_pipe && !has_redir);
    else if (is_str_in_set(cmd->argv[0], (char *[]){"echo", "pwd", "env", NULL}))
        return (!has_pipe && index == total_pipes); // only if no pipe at all
    return 0;
}

int execute_parent_process(int prev_fd, int *pipe_fds, int is_last)
{
    if (prev_fd != -1)
        close(prev_fd);
    if (!is_last)
        close(pipe_fds[1]);
    return (!is_last) ? pipe_fds[0] : -1;
}

pid_t handle_command_iteration(t_minishell *mini, char **envp,
                              t_cmd *cmd, int i, int prev_fd, int *pipe_fds)
{
    pid_t pid = -1;

    if (is_builtin(cmd->argv[0]) &&
        should_run_builtin_in_parent(cmd, i, mini->pipex_count))
    {
        if (cmd->in_type == HERE_DOC)
        {
            if (handle_heredoc(mini, cmd) < 0)
                return -1;
        }

        save_original_fds(cmd);  // Save fds before redirecting
        handle_redirections(cmd, prev_fd, pipe_fds, i == mini->pipex_count);
        execute_builtin(mini, i);
        restore_original_fds(cmd);  // Restore fds after builtin
        return -1; // no fork, no child
    }

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
        execute_child_process(mini, cmd, prev_fd, pipe_fds, i == mini->pipex_count, envp);

    execute_parent_process(prev_fd, pipe_fds, i == mini->pipex_count);
    return pid;
}



void execute_loop(t_minishell *mini, char **envp, pid_t *pids)
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
                execute_builtin(minishell, 0);
                continue;
            }
            else
            {
                char *path = resolve_cmd_path(minishell->cmd[i].argv[0], minishell->envp);
                // int j = i;
                // for (int h = 0; minishell->cmd[j].argv[h]; h++)
                // {
                //     printf("PATH: %s, CMD ARGV[%d][%d] = %s\n", path, j, h, minishell->cmd[j].argv[h]);
                // }
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

    }
}

void execute_command(t_minishell *mini, char **envp)
{
    if (mini->pipex_count == 0)
        execute_one_command(mini, envp);
    else
        multiple_command_execution(mini, envp);
}