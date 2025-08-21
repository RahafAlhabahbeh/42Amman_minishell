/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	redirect_input(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		write(2, "", 0);
		return (-1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	return (0);
}

int	redirect_output(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		write(2, "", 0);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	redirect_output_append(const char *file)
{
	int	fd;

	fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd((char *)file, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		write(2, "", 0);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}

int	handle_empty_redirections(t_cmd *cmd)
{
	if (handle_empty_output_files(cmd) < 0)
		return (-1);
	if (handle_empty_input_files(cmd) < 0)
		return (-1);
	if (handle_empty_redirections_list(cmd->redirections) < 0)
		return (-1);
	return (0);
}

int	handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last)
{
	t_redirection	*current;
	int				has_input_redir;
	int				has_output_redir;

	current = cmd->redirections;
	has_input_redir = 0;
	has_output_redir = 0;
	while (current)
	{
		if (process_single_redirection(current) == -1)
			return (-1);
		if (current->type == REDIR_IN)
			has_input_redir = 1;
		else if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
			has_output_redir = 1;
		current = current->next;
	}
	if (!has_input_redir && cmd->heredoc_fd >= 0)
		redirect_heredoc_input(cmd);
	else
		handle_pipe_input(prev_fd, has_input_redir);
	handle_pipe_output(pipe_fds, is_last, has_output_redir);
	return (0);
}
