/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#include <fcntl.h>

extern int	check_sigint_received(void);

static void	cleanup_previous_fd(int *last_fd, char **last_temp_file)
{
	if (*last_fd >= 0)
	{
		close(*last_fd);
		if (*last_temp_file)
			unlink(*last_temp_file);
	}
}

static int	handle_new_heredoc(t_minishell *mini, t_heredoc *current,
		int *last_fd, char **last_temp_file)
{
	*last_fd = create_heredoc_temp_file_with_quote(mini, current->delimiter,
			last_temp_file, current->quote);
	if (*last_fd < 0)
	{
		if (*last_temp_file)
			free(*last_temp_file);
		return (-1);
	}
	if (current->next)
	{
		close(*last_fd);
		unlink(*last_temp_file);
		free(*last_temp_file);
		*last_temp_file = NULL;
		*last_fd = -1;
	}
	return (0);
}

void	flush_stdin_after_heredocs(void)
{
	int		flags;
	char	buffer[1024];

	flags = fcntl(STDIN_FILENO, F_GETFL);
	if (flags == -1)
		return ;
	if (fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK) == -1)
		return ;
	while (read(STDIN_FILENO, buffer, sizeof(buffer)) > 0)
		;
	fcntl(STDIN_FILENO, F_SETFL, flags);
	rl_clear_pending_input();
}

int	process_multiple_heredocs(t_minishell *mini, t_cmd *cmd)
{
	t_heredoc	*current;
	char		*last_temp_file;
	int			last_fd;

	current = cmd->heredoc_list;
	last_temp_file = NULL;
	last_fd = -1;
	if (!current)
		return (0);
	while (current)
	{
		cleanup_previous_fd(&last_fd, &last_temp_file);
		if (handle_new_heredoc(mini, current, &last_fd, &last_temp_file) < 0)
			return (-1);
		current = current->next;
	}
	if (last_fd >= 0)
	{
		cmd->heredoc_fd = last_fd;
		cmd->heredoc_temp_file = last_temp_file;
		cmd->in_type = REDIR_IN;
		return (0);
	}
	return (-1);
}
