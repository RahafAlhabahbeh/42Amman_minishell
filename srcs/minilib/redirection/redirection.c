/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 02:44:27 by rahaf            ###   ########.fr       */
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



static int	process_single_redirection(t_redirection *redir)
{
	if (redir->type == REDIR_IN)
		return (redirect_input(redir->filename));
	else if (redir->type == REDIR_OUT)
		return (redirect_output(redir->filename));
	else if (redir->type == REDIR_APPEND)
		return (redirect_output_append(redir->filename));
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
	else if (!has_input_redir && prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (!has_output_redir && !is_last && pipe_fds && pipe_fds[0] != -1
		&& pipe_fds[1] != -1)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
	return (0);
}
