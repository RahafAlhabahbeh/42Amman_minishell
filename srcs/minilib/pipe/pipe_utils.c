/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
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
