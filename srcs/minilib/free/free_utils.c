/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+         +:+         */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:10:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 11:10:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	cleanup_child_process(t_minishell *mini)
{
	if (mini)
	{
		cleanup_heredoc_files(mini);
		if (mini->promp_input)
			free(mini->promp_input);
		if (mini->token)
			free_tokens(mini->token);
		if (mini->cmd)
			free_cmds_array(mini->cmd, mini->pipex_count);
		if (mini->env_list)
			free_env_list(mini->env_list);
	}
}

void	close_unused_fds(int start_fd, int max_fd)
{
	int	fd;

	fd = start_fd;
	while (fd < max_fd)
	{
		close(fd);
		fd++;
	}
}

void	close_extra_fds(int prev_fd, int *pipe_fds, int is_last)
{
	int	fd;

	fd = 3;
	while (fd < 256)
	{
		if ((fd == prev_fd)
			|| (pipe_fds && !is_last
				&& (fd == pipe_fds[0] || fd == pipe_fds[1])))
		{
			fd++;
			continue ;
		}
		close(fd);
		fd++;
	}
}
