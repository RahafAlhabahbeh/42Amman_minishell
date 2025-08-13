/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_command_execution.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	init_pids_array(pid_t *pids, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pids[i] = -1;
		i++;
	}
}

static void	wait_for_processes(t_minishell *mini, pid_t *pids, int count,
	int last_was_parent_builtin)
{
	int	status;
	int	i;

	i = 0;
	while (i < count)
	{
		if (pids[i] > 0)
		{
			waitpid(pids[i], &status, 0);
			if (!last_was_parent_builtin || i == count - 1)
			{
				if (WIFEXITED(status))
					mini->exit_status = WEXITSTATUS(status);
				else if (WIFSIGNALED(status))
				{
					mini->exit_status = 128 + WTERMSIG(status);
					if (WTERMSIG(status) == SIGINT)
					{
						write(1, "^C\n", 3);
						rl_replace_line("", 0);
						rl_on_new_line();
						rl_redisplay();
					}
				}
			}
		}
		else if (pids[i] == -2)
		{
			i++;
			continue ;
		}
		else if (pids[i] == 0)
		{
			i++;
			continue ;
		}
		i++;
	}
}

void	multiple_command_execution(t_minishell *mini, char **envp)
{
	int		count;
	pid_t	*pids;
	int		last_command_was_parent_builtin;

	count = mini->pipex_count + 1;
	pids = malloc(sizeof(pid_t) * count);
	if (!pids)
		return ;
	last_command_was_parent_builtin = 0;

	init_pids_array(pids, count);
	set_child_running(1);
	execute_loop(mini, envp, pids);
	if (pids[count - 1] == -2)
		last_command_was_parent_builtin = 1;
	wait_for_processes(mini, pids, count, last_command_was_parent_builtin);
	set_child_running(0);
	cleanup_heredoc_files(mini);
	free(pids);
}