/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 15:20:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_heredoc(t_minishell *mini, t_cmd *cmd)
{
	(void)mini;
	if (cmd->in_type != HERE_DOC)
		return (0);
	if (cmd->heredoc_fd >= 0)
	{
		cmd->in_type = REDIR_IN;
		return (0);
	}
	return (0);
}

static void	cleanup_single_heredoc(t_cmd *cmd)
{
	if (cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->heredoc_temp_file)
	{
		unlink(cmd->heredoc_temp_file);
		free(cmd->heredoc_temp_file);
		cmd->heredoc_temp_file = NULL;
	}
	if (cmd->heredoc_list)
	{
		free_heredoc_list(cmd->heredoc_list);
		cmd->heredoc_list = NULL;
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
		cleanup_single_heredoc(&cmd[i]);
		i++;
	}
}
