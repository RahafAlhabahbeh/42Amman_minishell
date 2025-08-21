/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_empty_output_files(t_cmd *cmd)
{
	int	fd;

	if (!cmd->output_file_name)
		return (0);
	if (cmd->out_type == REDIR_OUT)
		fd = open(cmd->output_file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (cmd->out_type == REDIR_APPEND)
		fd = open(cmd->output_file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else
		return (0);
	if (fd < 0)
	{
		perror(cmd->output_file_name);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_empty_input_files(t_cmd *cmd)
{
	int	fd;

	if (!cmd->input_file_name || cmd->in_type != REDIR_IN)
		return (0);
	fd = open(cmd->input_file_name, O_RDONLY);
	if (fd < 0)
	{
		perror(cmd->input_file_name);
		return (-1);
	}
	close(fd);
	return (0);
}

static int	handle_single_empty_redirection(t_redirection *redir)
{
	int	fd;

	if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else
		return (0);
	if (fd < 0)
	{
		perror(redir->filename);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_empty_redirections_list(t_redirection *current)
{
	while (current)
	{
		if (handle_single_empty_redirection(current) < 0)
			return (-1);
		current = current->next;
	}
	return (0);
}
