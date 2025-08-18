/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quote_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 21:03:32 by rahaf            ###   ########.fr       */
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
	memset(sa, 0, sizeof(*sa));
	sa->sa_handler = handle_heredoc_sigint;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, old_sa);
}

static int	process_line(t_minishell *mini, int fd,
		char *line, int expand_vars)
{
	char	*expanded;

	expanded = expand_heredoc_line(mini, line, expand_vars);
	free(line);
	if (!expanded)
		return (0);
	write(fd, expanded, ft_strlen(expanded));
	write(fd, "\n", 1);
	free(expanded);
	return (1);
}

static int	handle_heredoc_input(t_minishell *mini, int fd,
		const char *delimiter, int expand_vars)
{
	char	*line;
	size_t	line_size;
	ssize_t	read_size;

	line = NULL;
	line_size = 0;
	while (!check_sigint_received())
	{
		write(1, "> ", 2);
		read_size = ft_getline(&line, &line_size, 0);
		if (read_size < 0 || check_sigint_received())
		{
			free(line);
			mini->exit_status = 130;
			return (-1);
		}
		if (line[read_size - 1] == '\n')
			line[read_size - 1] = '\0';
		if (ft_strcmp(line, delimiter) == 0)
			return (free(line), 0);
		process_line(mini, fd, line, expand_vars);
		line = NULL;
	}
	return (0);
}

int	write_heredoc_lines(t_minishell *mini, int fd,
		const char *delimiter, int expand_vars)
{
	return (handle_heredoc_input(mini, fd, delimiter, expand_vars));
}
