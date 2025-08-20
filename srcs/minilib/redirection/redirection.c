/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:30:57 by rahaf            ###   ########.fr       */
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
		// Force flush stderr
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
		// Force flush stderr
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
		// Force flush stderr
		write(2, "", 0);
		return (-1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	return (0);
}



static int	handle_input_redirect(t_cmd *cmd, int prev_fd, int *pipe_fds)
{
	if (cmd->heredoc_fd >= 0)
		redirect_heredoc_input(cmd);
	else if (cmd->input_file_name)
	{
		if (redirect_input(cmd->input_file_name) < 0)
		{
			if (pipe_fds && pipe_fds[0] != -1)
				close(pipe_fds[0]);
			if (pipe_fds && pipe_fds[1] != -1)
				close(pipe_fds[1]);
			if (prev_fd != -1)
				close(prev_fd);
			return (-1);
		}
	}
	else if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	return (0);
}

int	handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last)
{
	if (handle_input_redirect(cmd, prev_fd, pipe_fds) == -1)
		return (-1);
	if (cmd->output_file_name)
	{
		if (cmd->out_type == REDIR_APPEND)
		{
			if (redirect_output_append(cmd->output_file_name) < 0)
				return (-1);
		}
		else if (redirect_output(cmd->output_file_name) < 0)
			return (-1);
	}
	else if (!is_last && pipe_fds && pipe_fds[0] != -1 && pipe_fds[1] != -1)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
	return (0);
}


