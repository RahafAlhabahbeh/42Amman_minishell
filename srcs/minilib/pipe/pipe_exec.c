/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	fork_and_execute(t_minishell *ms, t_pipe_data *data, int n)
{
	pid_t	pid;
	int		i;

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
		{
			data->i = i;
			execute_child_command_pipe(ms, data);
		}
		i++;
	}
}

static void	wait_for_children(int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}

void	execute_piped_commands(t_minishell *minishell, char **envp)
{
	int			n;
	int			*pipefds;
	t_pipe_data	data;

	n = minishell->pipex_count + 1;
	pipefds = init_pipes(n);
	data.pipefds = pipefds;
	data.n = n;
	data.envp = envp;
	fork_and_execute(minishell, &data, n);
	close_all_pipes(pipefds, n);
	wait_for_children(n);
	free(pipefds);
}
