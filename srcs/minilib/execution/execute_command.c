/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:02 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/27 14:16:54 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void execute_child_process(t_cmd *cmd, int prev_fd,
                                  int *pipe_fds, int is_last, char **envp)
{
    handle_redirections(cmd, prev_fd, pipe_fds, is_last);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    char *path = resolve_cmd_path(cmd->argv[0], envp);
    if (!path)
    {
        fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
        exit(127);
    }
    execve(path, cmd->argv, envp);
    perror("execve");
    exit(EXIT_FAILURE);
}

static int execute_parent_process(int prev_fd, int *pipe_fds, int is_last)
{
    if (prev_fd != -1)
        close(prev_fd);
    if (!is_last)
        close(pipe_fds[1]);
    return (!is_last) ? pipe_fds[0] : -1;
}

static int handle_command_iteration(t_minishell *mini, char **envp,
                                    t_cmd *cmd, int i, int prev_fd)
{
    int pipe_fds[2];
    pid_t pid;

    if (i < mini->pipex_count)
        safe_pipe(pipe_fds);

    if (is_builtin(cmd->argv[0]))
    {
        handle_redirections(cmd, prev_fd, pipe_fds, i == mini->pipex_count);
        execute_builtin(mini, i);
        return (i < mini->pipex_count) ? pipe_fds[0] : -1;
    }
    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid == 0)
        execute_child_process(cmd, prev_fd, pipe_fds, i == mini->pipex_count, envp);
    return execute_parent_process(prev_fd, pipe_fds, i == mini->pipex_count);
}

static void execute_loop(t_minishell *mini, char **envp)
{
    int prev_fd = -1;
    t_cmd *cmd = mini->cmd;

    for (int i = 0; cmd && i <= mini->pipex_count; i++, cmd = cmd->next)
        prev_fd = handle_command_iteration(mini, envp, cmd, i, prev_fd);
}

void execute_command(t_minishell *mini, char **envp)
{
    pid_t wpid;
    int status;

    execute_loop(mini, envp);

    while ((wpid = wait(&status)) > 0)
    {
        if (WIFEXITED(status))
            mini->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            mini->exit_status = 128 + WTERMSIG(status);
    }
}
