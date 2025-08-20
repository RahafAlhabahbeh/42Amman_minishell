/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 15:44:37 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	handle_input_fd_error(t_cmd *cmd)
{
	int	null_fd;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->input_file_name, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	null_fd = open("/dev/null", O_RDONLY);
	if (null_fd >= 0)
	{
		dup2(null_fd, STDIN_FILENO);
		close(null_fd);
	}
}

static void	handle_output_fd_error(t_cmd *cmd)
{
	int	null_fd;

	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(cmd->output_file_name, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	null_fd = open("/dev/null", O_WRONLY);
	if (null_fd >= 0)
	{
		dup2(null_fd, STDOUT_FILENO);
		close(null_fd);
	}
}

static void	redirect_input_pipe(t_cmd *cmd)
{
	int	fd_in;

	if (!cmd->input_file_name)
		return ;
	fd_in = open(cmd->input_file_name, O_RDONLY);
	if (fd_in == -1)
	{
		handle_input_fd_error(cmd);
		return ;
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2 error\n", 2);
		close(fd_in);
		return ;
	}
	close(fd_in);
}

static void	redirect_output_pipe(t_cmd *cmd)
{
	int	fd_out;

	if (!cmd->output_file_name)
		return ;
	fd_out = open(cmd->output_file_name,
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
	{
		handle_output_fd_error(cmd);
		return ;
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("minishell: dup2 error\n", 2);
		close(fd_out);
		return ;
	}
	close(fd_out);
}

void	handle_file_redirection(t_cmd *cmd)
{
	redirect_input_pipe(cmd);
	redirect_output_pipe(cmd);
}
