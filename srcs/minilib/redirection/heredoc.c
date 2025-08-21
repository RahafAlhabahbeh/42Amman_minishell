/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 02:30:07 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

extern int	check_sigint_received(void);

static void	cleanup_previous_fd(int *last_fd, char **last_temp_file)
{
	if (*last_fd >= 0)
	{
		close(*last_fd);
		if (*last_temp_file)
			unlink(*last_temp_file);
	}
}

static int	handle_new_heredoc(t_minishell *mini, t_heredoc *current,
		int *last_fd, char **last_temp_file)
{
	*last_fd = create_heredoc_temp_file_with_quote(mini, current->delimiter,
			last_temp_file, current->quote);
	if (*last_fd < 0)
	{
		if (*last_temp_file)
			free(*last_temp_file);
		return (-1);
	}
	if (current->next)
	{
		close(*last_fd);
		unlink(*last_temp_file);
		free(*last_temp_file);
		*last_temp_file = NULL;
		*last_fd = -1;
	}
	return (0);
}

int	process_multiple_heredocs(t_minishell *mini, t_cmd *cmd)
{
	t_heredoc	*current;
	char		*last_temp_file;
	int			last_fd;

	current = cmd->heredoc_list;
	last_temp_file = NULL;
	last_fd = -1;
	if (!current)
		return (0);
	while (current)
	{
		cleanup_previous_fd(&last_fd, &last_temp_file);
		if (handle_new_heredoc(mini, current, &last_fd, &last_temp_file) < 0)
			return (-1);
		current = current->next;
	}
	if (last_fd >= 0)
	{
		cmd->heredoc_fd = last_fd;
		cmd->heredoc_temp_file = last_temp_file;
		cmd->in_type = REDIR_IN;
		return (0);
	}
	return (-1);
}

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
