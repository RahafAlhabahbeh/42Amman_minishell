/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utilities.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 15:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	add_heredoc_to_list(t_cmd *cmd, const char *delimiter, char quote)
{
	t_heredoc	*new_heredoc;
	t_heredoc	*current;

	new_heredoc = malloc(sizeof(t_heredoc));
	if (!new_heredoc)
		return ;
	new_heredoc->delimiter = ft_strdup(delimiter);
	new_heredoc->quote = quote;
	new_heredoc->next = NULL;
	if (!cmd->heredoc_list)
		cmd->heredoc_list = new_heredoc;
	else
	{
		current = cmd->heredoc_list;
		while (current->next)
			current = current->next;
		current->next = new_heredoc;
	}
}

static int	cleanup_previous_heredoc(int last_fd, char *last_temp_file)
{
	if (last_fd >= 0)
		close(last_fd);
	if (last_temp_file)
	{
		unlink(last_temp_file);
		free(last_temp_file);
	}
	return (0);
}

static int	process_heredoc_node(t_minishell *mini, t_heredoc *node,
			char **last_temp_file, int *last_fd)
{
	if (*last_fd >= 0)
		cleanup_previous_heredoc(*last_fd, *last_temp_file);
	*last_fd = create_heredoc_temp_file_with_quote(mini,
			node->delimiter, last_temp_file, node->quote);
	if (*last_fd < 0)
	{
		if (*last_temp_file)
			free(*last_temp_file);
		return (-1);
	}
	return (0);
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
		if (process_heredoc_node(mini, current, &last_temp_file, &last_fd) < 0)
			return (-1);
		if (current->next)
			cleanup_previous_heredoc(last_fd, last_temp_file);
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
