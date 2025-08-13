/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_str_in_set(const char *s, char **set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (ft_strcmp(s, set[i]) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	is_redirection_present(t_cmd *cmd)
{
	return (cmd->in_type == REDIR_IN || cmd->in_type == HERE_DOC
		|| cmd->out_type == REDIR_OUT || cmd->out_type == REDIR_APPEND);
}

int	should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes)
{
	int		is_last;
	int		has_redir;
	char	*parent_builtins[3];

	is_last = (index == total_pipes);
	has_redir = is_redirection_present(cmd);
	parent_builtins[0] = "export";
	parent_builtins[1] = "unset";
	parent_builtins[2] = NULL;
	if (is_str_in_set(cmd->argv[0], parent_builtins))
		return (!has_redir && is_last && total_pipes == 0);
	if (ft_strcmp(cmd->argv[0], "cd") == 0)
		return (!has_redir && is_last);
	return (0);
}

int	execute_parent_process(int prev_fd, int *pipe_fds, int is_last)
{
	if (prev_fd != -1)
		close(prev_fd);
	if (!is_last)
		close(pipe_fds[1]);
	if (!is_last)
		return (pipe_fds[0]);
	return (-1);
}

static void	process_heredocs(t_minishell *mini)
{
	t_cmd	*cmd;
	int		i;

	i = 0;
	while (i <= mini->pipex_count)
	{
		cmd = &mini->cmd[i];
		if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
			return ;
		i++;
	}
}

static void	handle_empty_command(t_cmd *cmd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->in_type == REDIR_IN)
		{
			while (1)
			{
				bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (bytes_read <= 0)
					break ;
				write(STDOUT_FILENO, buffer, bytes_read);
			}
		}
		else
		{
			write(2, ": command not found\n", 20);
		}
		exit(127);
	}
}

static void	execute_child_command(t_minishell *mini, t_cmd *cmd, int i,
	char **envp)
{
	char	*path;
	int		status;
	char	*child_builtins[3];

	if (is_builtin(cmd->argv[0]))
	{
		child_builtins[0] = "export";
		child_builtins[1] = "unset";
		child_builtins[2] = NULL;
		if (is_str_in_set(cmd->argv[0], child_builtins)
			&& mini->pipex_count > 0)
		{
			cleanup_child_process(mini);
			exit(0);
		}
		execute_builtin(mini, i);
	}
	else
	{
		status = resolve_cmd_path_with_status(cmd->argv[0], mini, &path);
		if (status != 0)
		{
			if (status == 126)
			{
				if (is_directory(cmd->argv[0]))
				{
					write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
					write(2, ": Is a directory\n", 17);
				}
				else
				{
					write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
					write(2, ": Permission denied\n", 20);
				}
			}
			else
			{
				write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
				if (cmd->argv[0][0] == '/' || 
					(cmd->argv[0][0] == '.' && ft_strchr(cmd->argv[0], '/')))
					write(2, ": No such file or directory\n", 28);
				else
					write(2, ": command not found\n", 20);
			}
			cleanup_child_process(mini);
			exit(status);
		}
		execve(path, cmd->argv, envp);
		perror("execve");
		free(path);
		cleanup_child_process(mini);
		exit(126);
	}
	cleanup_child_process(mini);
	exit(0);
}

static void	handle_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipefd, int i, char **envp)
{
	set_in_child_process(1);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (handle_redirections(cmd, prev_fd, pipefd,
			i == mini->pipex_count) < 0)
		exit(1);
	handle_empty_command(cmd);
	execute_child_command(mini, cmd, i, envp);
}

static int	handle_parent_builtin(t_minishell *mini, t_cmd *cmd, int prev_fd,
	int *pipefd, int i, pid_t *pids)
{
	if (is_builtin(cmd->argv[0])
		&& should_run_builtin_in_parent(cmd, i, mini->pipex_count))
	{
		save_original_fds(cmd);
		if (handle_redirections(cmd, prev_fd, pipefd,
				i == mini->pipex_count) == 0)
			execute_builtin(mini, i);
		restore_original_fds(cmd);
		pids[i] = -2;
		return (1);
	}
	return (0);
}

void	execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
	int		i;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	t_cmd	*cmd;

	prev_fd = -1;
	process_heredocs(mini);
	i = 0;
	while (i <= mini->pipex_count)
	{
		if (i < mini->pipex_count && pipe(pipefd) == -1)
		{
			perror("pipe");
			mini->exit_status = 1;
			exit(EXIT_FAILURE);
		}
		cmd = &mini->cmd[i];
		if (handle_parent_builtin(mini, cmd, prev_fd, pipefd, i, pids))
		{
			prev_fd = execute_parent_process(prev_fd, pipefd,
					i == mini->pipex_count);
			i++;
			continue ;
		}
		pid = fork();
		if (pid == -1)
		{
			set_child_running(0);
			perror("fork");
			mini->exit_status = 1;
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			handle_child_process(mini, cmd, prev_fd, pipefd, i, envp);
		else
		{
			pids[i] = pid;
			prev_fd = execute_parent_process(prev_fd, pipefd,
					i == mini->pipex_count);
		}
		i++;
	}
}

void	execute_command(t_minishell *mini, char **envp)
{
	if (mini->pipex_count == 0)
		execute_one_command(mini, envp);
	else
		multiple_command_execution(mini, envp);
}