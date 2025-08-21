/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_pipes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_pipe_input(int prev_fd, int has_input_redir)
{
	if (!has_input_redir && prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
}

void	handle_pipe_output(int *pipe_fds, int is_last, int has_output_redir)
{
	if (!has_output_redir && !is_last && pipe_fds && pipe_fds[0] != -1
		&& pipe_fds[1] != -1)
	{
		close(pipe_fds[0]);
		dup2(pipe_fds[1], STDOUT_FILENO);
		close(pipe_fds[1]);
	}
}

int	process_single_redirection(t_redirection *redir)
{
	if (redir->type == REDIR_IN)
		return (redirect_input(redir->filename));
	else if (redir->type == REDIR_OUT)
		return (redirect_output(redir->filename));
	else if (redir->type == REDIR_APPEND)
		return (redirect_output_append(redir->filename));
	return (0);
}
