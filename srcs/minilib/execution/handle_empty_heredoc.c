/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_empty_heredoc.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 14:37:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 14:37:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	handle_empty_heredoc_command(t_minishell *mini, t_exec_vars *vars,
	char **child_env)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		heredoc_fd;

	(void)mini;
	(void)child_env;
	if (!vars->cmd->input_file_name)
		return ;
	heredoc_fd = open(vars->cmd->input_file_name, O_RDONLY);
	if (heredoc_fd < 0)
		return ;
	bytes_read = read(heredoc_fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(heredoc_fd, buffer, sizeof(buffer));
	}
	close(heredoc_fd);
}
