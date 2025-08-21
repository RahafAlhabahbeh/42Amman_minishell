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
	int	heredoc_fd;
	int	result;

	if (cmd->in_type != HERE_DOC)
		return (0);
	if (cmd->heredoc_list)
	{
		result = process_multiple_heredocs(mini, cmd);
		if (result >= 0)
			flush_stdin_after_heredocs();
		return (result);
	}
	if (!cmd->input_file_name)
		return (0);
	heredoc_fd = create_heredoc_temp_file_with_quote(mini,
			cmd->input_file_name, &cmd->heredoc_temp_file, cmd->input_quote);
	if (heredoc_fd < 0)
	{
		mini->exit_status = 1;
		return (-1);
	}
	cmd->heredoc_fd = heredoc_fd;
	cmd->in_type = REDIR_IN;
	flush_stdin_after_heredocs();
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
