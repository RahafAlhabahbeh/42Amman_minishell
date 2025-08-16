/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	redirect_input_pipe(t_cmd *cmd)
{
	int	fd_in;

	if (!cmd->input_file_name)
		return ;
	fd_in = open(cmd->input_file_name, O_RDONLY);
	if (fd_in == -1)
	{
		perror("open input file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2 input");
		exit(EXIT_FAILURE);
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
		perror("open output file");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2 output");
		exit(EXIT_FAILURE);
	}
	close(fd_out);
}

void	handle_file_redirection(t_cmd *cmd)
{
	redirect_input_pipe(cmd);
	redirect_output_pipe(cmd);
}
