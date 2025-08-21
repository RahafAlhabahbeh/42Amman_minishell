/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_empty_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 03:59:30 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_empty_command(t_minishell *mini, t_cmd *cmd, char **child_env)
{
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->in_type == REDIR_IN || cmd->out_type == REDIR_OUT
			|| cmd->out_type == REDIR_APPEND
			|| cmd->in_type == HERE_DOC || cmd->redirections)
		{
			free_env_array_2(child_env);
			cleanup_child_process(mini);
			exit(0);
		}
		else
		{
			ft_putstr_fd("minishell: ", 2);
			if (cmd->argv && cmd->argv[0])
				ft_putstr_fd(cmd->argv[0], 2);
			else
				ft_putstr_fd("command", 2);
			ft_putstr_fd(": command not found\n", 2);
			free_env_array_2(child_env);
			cleanup_child_process(mini);
			exit(127);
		}
	}
}
