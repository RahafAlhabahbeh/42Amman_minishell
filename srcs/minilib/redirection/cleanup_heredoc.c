/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 22:10:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_heredoc_sigint(int sig)
{
	g_received_signal = sig;
	write(1, "\n", 1);
}

void	free_heredoc_list(t_heredoc *list)
{
	t_heredoc	*current;
	t_heredoc	*next;

	current = list;
	while (current)
	{
		next = current->next;
		free(current->delimiter);
		free(current);
		current = next;
	}
}

void	cleanup_heredoc_files(t_minishell *mini)
{
	t_cmd	*cmd;
	int		i;

	if (!mini || !mini->cmd)
		return ;
	cmd = mini->cmd;
	i = 0;
	while (i <= mini->pipex_count)
	{
		if (cmd[i].heredoc_fd >= 0)
		{
			close(cmd[i].heredoc_fd);
			cmd[i].heredoc_fd = -1;
		}
		if (cmd[i].heredoc_temp_file)
		{
			unlink(cmd[i].heredoc_temp_file);
			free(cmd[i].heredoc_temp_file);
			cmd[i].heredoc_temp_file = NULL;
		}
		if (cmd[i].heredoc_list)
		{
			free_heredoc_list(cmd[i].heredoc_list);
			cmd[i].heredoc_list = NULL;
		}
		i++;
	}
}
