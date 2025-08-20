/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 16:07:32 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	*init_pipes(int n)
{
	int	*pipefds;
	int	i;

	pipefds = malloc(sizeof(int) * 2 * (n - 1));
	if (!pipefds)
		exit(EXIT_FAILURE);
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
	return (pipefds);
}

void	close_all_pipes(int *pipefds, int n)
{
	int	j;

	j = 0;
	while (j < 2 * (n - 1))
	{
		close(pipefds[j]);
		j++;
	}
}

void	setup_child_pipes(int *pipefds, int i, int n)
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

void	close_pipe_fds(int *pipe_fds)
{
	if (pipe_fds[0] != -1)
	{
		close(pipe_fds[0]);
		pipe_fds[0] = -1;
	}
	if (pipe_fds[1] != -1)
	{
		close(pipe_fds[1]);
		pipe_fds[1] = -1;
	}
}

int	create_pipe_if_needed(t_minishell *mini, t_exec_vars *vars)
{
	if (vars->i < mini->pipex_count && pipe(vars->pipefd) == -1)
	{
		perror("pipe");
		mini->exit_status = 1;
		return (1);
	}
	return (0);
}
