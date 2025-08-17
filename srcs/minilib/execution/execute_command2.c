/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
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
	(void)cmd;
	(void)index;
	(void)total_pipes;
	return (0);
}

int	execute_parent_process(t_exec_vars *vars, int is_last)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	if (vars->prev_fd != -1)
		close(vars->prev_fd);
	if (!is_last)
	{
		close(vars->pipefd[1]);
		vars->pipefd[1] = -1;
		return (vars->pipefd[0]);
	}
	else
		close_pipe_fds(vars->pipefd);
	return (-1);
}
/*
int	execute_parent_process(int prev_fd, int *pipe_fds, int is_last)
{
	struct sigaction	sa;

	ft_memset(&sa, 0, sizeof(sa));
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	if (prev_fd != -1)
		close(prev_fd);
	if (!is_last)
	{
		close(pipe_fds[1]);
		pipe_fds[1] = -1;
		return (pipe_fds[0]);
	}
	else
		close_pipe_fds(pipe_fds);
	return (-1);
}
*/
