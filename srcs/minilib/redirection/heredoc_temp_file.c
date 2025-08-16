/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_temp_file.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 23:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	setup_temp_file(char **filename_ptr, int *fd, int counter)
{
	char	temp[256];
	char	*pid_str;
	char	*count_str;

	ft_strlcpy(temp, "/tmp/heredoc_", sizeof(temp));
	pid_str = ft_itoa(getpid());
	ft_strlcat(temp, pid_str, sizeof(temp));
	free(pid_str);
	count_str = ft_itoa(counter);
	ft_strlcat(temp, "_", sizeof(temp));
	ft_strlcat(temp, count_str, sizeof(temp));
	free(count_str);
	*filename_ptr = ft_strdup(temp);
	if (!*filename_ptr)
		return (-1);
	*fd = open(temp, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (*fd < 0)
	{
		perror("minishell: heredoc temp file creation failed");
		return (-1);
	}
	return (0);
}

static void	setup_signal(struct sigaction *sa, struct sigaction *old_sa)
{
	memset(sa, 0, sizeof(*sa));
	sa->sa_handler = handle_heredoc_sigint;
	sigemptyset(&sa->sa_mask);
	sa->sa_flags = 0;
	sigaction(SIGINT, sa, old_sa);
}

static void	write_heredoc_lines(t_minishell *mini, int fd,
	char *delimiter, int expand_vars)
{
	char	*line;
	size_t	size;
	ssize_t	read_size;

	line = NULL;
	size = 0;
	while (!check_sigint_received())
	{
		read_size = read_heredoc_line(mini, &line, &size);
		if (read_size < 0)
			break ;
		if (ft_strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		line = expand_heredoc_line(mini, line, expand_vars);
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
		line = NULL;
	}
}

int	create_heredoc_temp_file_with_quote(t_minishell *mini,
	const char *delimiter, char **filename_ptr, char quote_char)
{
	static int			counter = 0;
	int					fd;
	char				*clean;
	struct sigaction	sa;
	struct sigaction	old_sa;

	if (setup_temp_file(filename_ptr, &fd, counter++) < 0)
		return (-1);
	clean = ft_strdup(delimiter);
	if (!clean)
	{
		close(fd);
		unlink(*filename_ptr);
		return (-1);
	}
	setup_signal(&sa, &old_sa);
	write_heredoc_lines(mini, fd, clean,
		(quote_char != '\'' && quote_char != '"'));
	close(fd);
	free(clean);
	sigaction(SIGINT, &old_sa, NULL);
	if (check_sigint_received())
		return (-1);
	return (open(*filename_ptr, O_RDONLY));
}
