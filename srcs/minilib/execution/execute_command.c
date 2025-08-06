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

int is_str_in_set(const char *s, char **set)
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
    return (cmd->in_type == REDIR_IN || cmd->in_type == HERE_DOC ||
            cmd->out_type == REDIR_OUT || cmd->out_type == REDIR_APPEND);
}

int should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes)
{
    int is_last = (index == total_pipes);
    int has_redir = is_redirection_present(cmd);

    // These builtins must be run in parent to affect shell state
    if (is_str_in_set(cmd->argv[0], (char *[]) {"export", "unset", "exit", NULL}))
        return (!has_redir && is_last);

    // Special case for 'cd': run in parent if it is last and has no redirection
    if (ft_strcmp(cmd->argv[0], "cd") == 0)
    {
        fprintf(stderr, "Running 'cd' in parent\n");
        return (!has_redir && is_last);
    }

    // Other builtins (like echo, pwd, env) don't need to run in parent
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

void execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
    int i;
    int pipefd[2];
    int prev_fd = -1;
    pid_t pid;

    // Process all heredocs in parent before forking
    for (i = 0; i <= mini->pipex_count; i++)
    {
        t_cmd *cmd = &mini->cmd[i];
        if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
            return; // Exit if heredoc fails
    }

    for (i = 0; i <= mini->pipex_count; i++)
    {
        if (i < mini->pipex_count && pipe(pipefd) == -1)
        {
            perror("pipe");
            mini->exit_status = 1;  // General error
            exit(EXIT_FAILURE);
        }

        t_cmd *cmd = &mini->cmd[i];

        // Handle builtins that should run in parent process (no pipes, no redirections)
        if (is_builtin(cmd->argv[0]) && should_run_builtin_in_parent(cmd, i, mini->pipex_count))
        {
            save_original_fds(cmd);
            if (handle_redirections(cmd, prev_fd, pipefd, i == mini->pipex_count) == 0)
                execute_builtin(mini, i);
            restore_original_fds(cmd);
            prev_fd = execute_parent_process(prev_fd, pipefd, i == mini->pipex_count);
            continue;
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            mini->exit_status = 1;  // General error
            exit(EXIT_FAILURE);
        }
        else if (pid == 0)
        {
            signal(SIGINT, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);
            if (handle_redirections(cmd, prev_fd, pipefd, i == mini->pipex_count) < 0)
                exit(1);
            
            // Handle empty command (just pass heredoc content through)
            if (!cmd->argv || !cmd->argv[0])
            {
                // For empty command with heredoc, we need to pass the content through
                // The redirections have already been set up, so just read from stdin and write to stdout
                if (cmd->in_type == REDIR_IN)
                {
                    char buffer[1024];
                    ssize_t bytes_read;
                    while ((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer))) > 0)
                    {
                        write(STDOUT_FILENO, buffer, bytes_read);
                    }
                }
                exit(0);
            }
            
            if (is_builtin(cmd->argv[0]))
                execute_builtin(mini, i);
            else
            {
                char *path = resolve_cmd_path(cmd->argv[0], envp);
                if (!path)
                {
                    fprintf(stderr, "%s: command not found\n", cmd->argv[0]);
                    exit(127);
                }
                execve(path, cmd->argv, envp);
                perror("execve");
                exit(126);  // Command invoked cannot execute
            }
            exit(0);
        }
        else
        {
            pids[i] = pid;
            prev_fd = execute_parent_process(prev_fd, pipefd, i == mini->pipex_count);
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