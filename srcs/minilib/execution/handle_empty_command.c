/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_empty_command(t_cmd *cmd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		if (cmd->in_type == REDIR_IN)
		{
			while (1)
			{
				bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer));
				if (bytes_read <= 0)
					break ;
				write(STDOUT_FILENO, buffer, bytes_read);
			}
			exit(0);
		}
		else
		{
			write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
			write(2, ": command not found\n", 20);
			exit(127);
		}
	}
}
