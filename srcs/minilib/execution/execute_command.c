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

void execute_builtin_cmd(t_minishell *mini, t_cmd *cmd)
{
    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    char *name = cmd->argv[0];

    if (!ft_strcmp(name, "echo"))
        call_echo(mini, cmd->argv);
    else if (!ft_strcmp(name, "pwd"))
        call_pwd(mini);
    else if (!ft_strcmp(name, "env"))
        call_env(mini, cmd->argv);
}


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

static int is_redirection_present(t_cmd *cmd)
{
    if (cmd->in_type == REDIR_IN || cmd->in_type == HERE_DOC)
        return 1;
    if (cmd->out_type == REDIR_OUT || cmd->out_type == REDIR_APPEND)
        return 1;
    return 0;
}


static int should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes)
{
    int has_pipe = (index > 0) || (index < total_pipes);
    int has_redir = is_redirection_present(cmd);

    if (is_str_in_set(cmd->argv[0], (char *[]){"export", "unset", "cd", "exit", NULL}))
        return (!has_pipe && !has_redir);
    else if (is_str_in_set(cmd->argv[0], (char *[]){"echo", "pwd", "env", NULL}))
        return (index == total_pipes); // only if no pipe *after*
    return 0;
}

static void execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
                                  int *pipe_fds, int is_last, char **envp)
{
    //handle_redirections(cmd, prev_fd, pipe_fds, is_last);
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    if (is_builtin(cmd->argv[0]))
    {
        fprintf(stdout, "Build in child\n");
        handle_redirections(cmd, prev_fd, pipe_fds, is_last);
        execute_builtin_cmd(mini, cmd);
        free_commands(mini->cmd, mini->pipex_count);
        free_tokens(mini->token);
        free(mini->promp_input);
        exit(0);
    }

    handle_redirections(cmd, prev_fd, pipe_fds, is_last);
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

static pid_t handle_command_iteration(t_minishell *mini, char **envp,
                                      t_cmd *cmd, int i, int prev_fd)
{
    int pipe_fds[2];
    pid_t pid = -1;

    if (i < mini->pipex_count)
        safe_pipe(pipe_fds);

    if (is_builtin(cmd->argv[0]) &&
        should_run_builtin_in_parent(cmd, i, mini->pipex_count))
    {
        handle_redirections(cmd, prev_fd, pipe_fds, i == mini->pipex_count);
        execute_builtin(mini, i);
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


static void execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
    int prev_fd = -1;
    t_cmd *cmd = mini->cmd;

    for (int i = 0; cmd && i <= mini->pipex_count; i++, cmd = cmd->next)
    {
        pid_t pid = handle_command_iteration(mini, envp, cmd, i, prev_fd);
        if (pid > 0) // Only save real child PIDs
            pids[i] = pid;
        prev_fd = (pid == 0) ? -1 : prev_fd; // Child has already exited
    }
}

void execute_command(t_minishell *mini, char **envp)
{
    int count = mini->pipex_count + 1;
    pid_t pids[count];
    int status;

    ft_bzero(pids, sizeof(pids)); // make sure to zero out

    execute_loop(mini, envp, pids);

    for (int i = 0; i < count; i++)
    {
        if (pids[i] > 0)
        {
            waitpid(pids[i], &status, 0);
            if (WIFEXITED(status))
                mini->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                mini->exit_status = 128 + WTERMSIG(status);
        }
    }
}

