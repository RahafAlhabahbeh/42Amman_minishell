/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quote_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 16:05:06 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	close_all_heredoc_fds(t_minishell *mini)
{
	t_cmd	*cmd;
	int		i;

	if (!mini || !mini->cmd)
		return ;
	cmd = mini->cmd;
	i = 0;
	while (i <= mini->pipex_count)
	{
		if (cmd[i].heredoc_fd >= 0)
		{
			close(cmd[i].heredoc_fd);
			cmd[i].heredoc_fd = -1;
		}
		i++;
	}
}

void	setup_heredoc_signal(struct sigaction *sa, struct sigaction *old_sa)
{
	ft_memset(sa, 0, sizeof(*sa));
	sa->sa_handler = handle_heredoc_sigint;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, old_sa);
}

static int	handle_input_error(t_minishell *mini, char *line,
		ssize_t read_size, const char *delimiter)
{
	if (read_size < 0 && !check_sigint_received())
	{
		if (!isatty(0))
		{
			free(line);
			return (0);
		}
		return (handle_eof_warning(delimiter, line));
	}
	free(line);
	mini->exit_status = 130;
	return (-1);
}

static int	read_and_check_line(char **line, size_t *line_size,
	const char *delimiter)
{
	ssize_t	read_size;

	read_size = ft_getline(line, line_size, 0);
	if (read_size < 0 || check_sigint_received())
		return (read_size);
	if ((*line)[read_size - 1] == '\n')
		(*line)[read_size - 1] = '\0';
	if (ft_strcmp(*line, delimiter) == 0)
	{
		free(*line);
		return (0);
	}
	return (read_size);
}

int	handle_heredoc_input(t_minishell *mini, int fd,
		const char *delimiter, int expand_vars)
{
	char	*line;
	size_t	line_size;
	ssize_t	result;

	line = NULL;
	line_size = 0;
	while (!check_sigint_received())
	{
		if (isatty(0))
			write(1, "> ", 2);
		result = read_and_check_line(&line, &line_size, delimiter);
		if (result < 0 || check_sigint_received())
		{
			if (result < 0 && !isatty(0) && !check_sigint_received())
				return (free(line), 0);
			return (handle_input_error(mini, line, result, delimiter));
		}
		if (result == 0)
			return (0);
		process_line(mini, fd, line, expand_vars);
		free(line);
		line = NULL;
	}
	if (line)
		free(line);
	return (0);
}
