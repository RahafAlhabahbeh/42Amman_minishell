/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	safe_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	count_pipe(t_minishell *minishell)
{
	int		count;
	t_token	*cur;

	count = 0;
	cur = minishell->token;
	while (cur)
	{
		if (cur->type == PIPE)
			count++;
		cur = cur->next;
	}
	minishell->pipex_count = count;
}

static void	create_pipes(int *pipefds, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipefds + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

static void	setup_child_pipes(int *pipefds, int i, int n)
{
	if (i != 0)
	{
		if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (i != n - 1)
	{
		if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

static void	close_all_pipes(int *pipefds, int n)
{
	int	j;

	j = 0;
	while (j < 2 * (n - 1))
	{
		close(pipefds[j]);
		j++;
	}
}

static void	handle_file_redirection(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	if (cmd->input_file_name)
	{
		fd_in = open(cmd->input_file_name, O_RDONLY);
		if (fd_in == -1)
		{
			perror("open input file");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
		close(fd_in);
	}
	if (cmd->output_file_name)
	{
		fd_out = open(cmd->output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
		{
			perror("open output file");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(EXIT_FAILURE);
		}
		close(fd_out);
	}
}

static void	execute_child_command(t_minishell *minishell, int *pipefds,
	int i, int n, char **envp)
{
	setup_child_pipes(pipefds, i, n);
	close_all_pipes(pipefds, n);
	handle_file_redirection(&minishell->cmd[i]);
	execve(minishell->cmd[i].argv[0], minishell->cmd[i].argv, envp);
	perror("execve");
	exit(EXIT_FAILURE);
}

void	execute_piped_commands(t_minishell *minishell, char **envp)
{
	int		n;
	int		pipefds[2 * (minishell->pipex_count)];
	pid_t	pid;
	int		i;

	n = minishell->pipex_count + 1;
	create_pipes(pipefds, n);
	i = 0;
	while (i < n)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
			execute_child_command(minishell, pipefds, i, n, envp);
		i++;
	}
	close_all_pipes(pipefds, n);
	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}