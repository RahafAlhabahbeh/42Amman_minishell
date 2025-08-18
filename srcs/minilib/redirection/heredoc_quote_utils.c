/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_quote_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 14:30:00 by rahaf            ###   ########.fr       */
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

char	*generate_heredoc_filename(int counter)
{
	char	*filename;
	char	pid_str[12];
	char	counter_str[12];

	ft_strlcpy(pid_str, ft_itoa(getpid()), sizeof(pid_str));
	ft_strlcpy(counter_str, ft_itoa(counter), sizeof(counter_str));
	filename = malloc(256);
	if (!filename)
		return (NULL);
	ft_strlcpy(filename, "/tmp/heredoc_", 256);
	ft_strlcat(filename, pid_str, 256);
	ft_strlcat(filename, "_", 256);
	ft_strlcat(filename, counter_str, 256);
	return (filename);
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

int	write_heredoc_lines(t_minishell *mini, int fd,
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
