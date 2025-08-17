/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 21:50:55 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	save_original_fds(t_cmd *cmd)
{
	cmd->original_stdin = dup(STDIN_FILENO);
	cmd->original_stdout = dup(STDOUT_FILENO);
}

void	restore_original_fds(t_cmd *cmd)
{
	if (cmd->original_stdin != -1)
		dup2(cmd->original_stdin, STDIN_FILENO);
	if (cmd->original_stdout != -1)
		dup2(cmd->original_stdout, STDOUT_FILENO);
	if (cmd->original_stdin != -1)
		close(cmd->original_stdin);
	if (cmd->original_stdout != -1)
		close(cmd->original_stdout);
	cmd->original_stdin = -1;
	cmd->original_stdout = -1;
}
