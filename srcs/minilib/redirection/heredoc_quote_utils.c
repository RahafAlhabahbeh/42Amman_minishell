/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quote_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 16:34:12 by rahaf            ###   ########.fr       */
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

static int	handle_heredoc_errors(char **line, ssize_t result,
	const char *delimiter)
{
	if (check_sigint_received())
		return (free(*line), -1);
	if (result < 0)
	{
		if (!isatty(0))
			return (free(*line), 0);
		return (handle_eof_warning(delimiter, *line));
	}
	return (1);
}

int	handle_heredoc_input(t_minishell *mini, int fd,
		const char *delimiter, int expand_vars)
{
	char	*line;
	size_t	line_size;
	ssize_t	result;
	int		error_result;

	line = NULL;
	line_size = 0;
	while (!check_sigint_received())
	{
		if (isatty(0))
			write(1, "> ", 2);
		result = read_and_check_line(&line, &line_size, delimiter);
		error_result = handle_heredoc_errors(&line, result, delimiter);
		if (error_result == -1)
			return (mini->exit_status = 130, -1);
		if (error_result <= 0 || result == 0)
			return (error_result);
		process_line(mini, fd, line, expand_vars);
		free(line);
		line = NULL;
	}
	return (free(line), 0);
}
