/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_heredoc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 21:20:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	create_heredoc_temp_file(t_minishell *mini, const char *delimiter,
		char **temp_filename_ptr, char quote_char)
{
	int			fd;
	int			expand_vars;
	static int	counter = 0;

	*temp_filename_ptr = generate_temp_filename(counter++);
	if (!*temp_filename_ptr)
		return (-1);
	fd = open_heredoc_file(*temp_filename_ptr);
	if (fd < 0)
		return (-1);
	expand_vars = (quote_char != '\'' && quote_char != '"');
	if (write_heredoc_content(mini, fd, delimiter, expand_vars) < 0)
	{
		close(fd);
		unlink(*temp_filename_ptr);
		return (-1);
	}
	close(fd);
	return (open(*temp_filename_ptr, O_RDONLY));
}

void	redirect_heredoc_input(t_cmd *cmd)
{
	if (!cmd || cmd->heredoc_fd < 0)
		return ;
	dup2(cmd->heredoc_fd, STDIN_FILENO);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
}

void	close_unused_heredoc_fds(t_minishell *mini, t_cmd *current_cmd)
{
	t_cmd	*cmd;
	int		i;

	if (!mini || !mini->cmd)
		return ;
	cmd = mini->cmd;
	i = 0;
	while (i <= mini->pipex_count)
	{
		if (&cmd[i] != current_cmd && cmd[i].heredoc_fd >= 0)
		{
			close(cmd[i].heredoc_fd);
			cmd[i].heredoc_fd = -1;
		}
		i++;
	}
}

void	close_all_heredoc_fds(t_minishell *mini)
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
		i++;
	}
}
