/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_signal.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 15:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_heredoc_sigint(int sig)
{
	(void)sig;
	g_received_signal = SIGINT;
	write(1, "^C\n", 3);
}

void	append_to_result(char *result, char *value, int *j)
{
	int	len;

	len = ft_strlen(value);
	if (*j + len < 1023)
	{
		ft_strlcpy(result + *j, value, 1024 - *j);
		*j += len;
	}
	free(value);
}

void	redirect_heredoc_input(t_cmd *cmd)
{
	if (!cmd || cmd->heredoc_fd < 0)
		return ;
	dup2(cmd->heredoc_fd, STDIN_FILENO);
	close(cmd->heredoc_fd);
	cmd->heredoc_fd = -1;
}

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
