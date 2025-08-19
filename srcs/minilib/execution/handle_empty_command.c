/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_empty_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/19 15:32:46 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_empty_command(t_cmd *cmd, char **envp)
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
			free_env_array_2(envp);
			exit(0);
		}
		else
		{
			write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
			write(2, ": command not found\n", 20);
			free_env_array_2(envp);
			exit(127);
		}
	}
}
