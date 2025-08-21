/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 12:30:07 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_eof_warning(const char *delimiter, char *line)
{
	write(2, "minishell: warning: here-document delimited by ", 48);
	write(2, "end-of-file (wanted `", 21);
	write(2, delimiter, ft_strlen(delimiter));
	write(2, "')\n", 3);
	free(line);
	return (0);
}

int	process_line(t_minishell *mini, int fd, char *line, int expand_vars)
{
	char	*expanded;

	if (!expand_vars)
	{
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		return (1);
	}
	expanded = expand_heredoc_line(mini, line, expand_vars);
	if (!expanded)
		return (0);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (1);
}

void	cleanup_previous_heredoc(t_minishell *mini, int cmd_index)
{
	if (mini->cmd[cmd_index].heredoc_fd >= 0)
	{
		close(mini->cmd[cmd_index].heredoc_fd);
		mini->cmd[cmd_index].heredoc_fd = -1;
		if (mini->cmd[cmd_index].input_file_name)
		{
			unlink(mini->cmd[cmd_index].input_file_name);
			free(mini->cmd[cmd_index].input_file_name);
			mini->cmd[cmd_index].input_file_name = NULL;
		}
	}
}
