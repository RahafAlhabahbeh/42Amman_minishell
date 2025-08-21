/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_filename.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 05:50:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*create_filename_with_pid(char *pid_str, char *counter_str)
{
	char	*filename;

	filename = malloc(256);
	if (!filename)
		return (NULL);
	ft_strlcpy(filename, "/tmp/heredoc_", 256);
	ft_strlcat(filename, pid_str, 256);
	ft_strlcat(filename, "_", 256);
	ft_strlcat(filename, counter_str, 256);
	return (filename);
}

char	*generate_heredoc_filename(int counter)
{
	char	*filename;
	char	pid_str[12];
	char	counter_str[12];
	char	*temp_pid;
	char	*temp_counter;

	temp_pid = ft_itoa(1000 + counter % 9000);
	if (!temp_pid)
		return (NULL);
	temp_counter = ft_itoa(counter);
	if (!temp_counter)
		return (free(temp_pid), NULL);
	ft_strlcpy(pid_str, temp_pid, sizeof(pid_str));
	ft_strlcpy(counter_str, temp_counter, sizeof(counter_str));
	free(temp_pid);
	free(temp_counter);
	filename = create_filename_with_pid(pid_str, counter_str);
	return (filename);
}
