/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/14 21:27:12 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

extern int	check_sigint_received(void);

char	*generate_temp_filename(int counter)
{
	char	*filename;
	char	*pid_str;
	char	*count_str;
	char	temp[256];

	ft_strlcpy(temp, "/tmp/heredoc_", sizeof(temp));
	pid_str = ft_itoa(getpid());
	ft_strlcat(temp, pid_str, sizeof(temp));
	free(pid_str);
	count_str = ft_itoa(counter);
	ft_strlcat(temp, "_", sizeof(temp));
	ft_strlcat(temp, count_str, sizeof(temp));
	free(count_str);
	filename = ft_strdup(temp);
	return (filename);
}

int	open_heredoc_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		perror("minishell: heredoc temp file creation failed");
	return (fd);
}

ssize_t	read_heredoc_line(t_minishell *mini, char **line,
	size_t *line_size)
{
	ssize_t	read_size;

	write(1, "> ", 2);
	read_size = ft_getline(line, line_size, 0);
	if (read_size < 0 || check_sigint_received())
	{
		free(*line);
		*line = NULL;
		mini->exit_status = 130;
		return (-1);
	}
	if ((*line)[read_size - 1] == '\n')
		(*line)[read_size - 1] = '\0';
	return (read_size);
}

int	write_heredoc_content(t_minishell *mini,
		int fd, const char *delimiter, int expand_vars)
{
	char	*line;
	size_t	line_size;
	ssize_t	read_size;

	line = NULL;
	line_size = 0;
	while (!check_sigint_received())
	{
		read_size = read_heredoc_line(mini, &line, &line_size);
		if (read_size < 0)
			return (-1);
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
	return (0);
}

int	handle_heredoc(t_minishell *mini, t_cmd *cmd)
{
	int		heredoc_fd;

	if (cmd->in_type != HERE_DOC)
		return (0);
	if (!cmd->input_file_name)
		return (0);
	heredoc_fd = create_heredoc_temp_file(mini,
			cmd->input_file_name, &cmd->heredoc_temp_file,
			cmd->input_quote);
	if (heredoc_fd < 0)
	{
		mini->exit_status = 1;
		return (-1);
	}
	cmd->heredoc_fd = heredoc_fd;
	cmd->in_type = REDIR_IN;
	return (0);
}
/*
static int	create_heredoc_temp_file_with_quote(t_minishell *mini,
			const char *delimiter, char **temp_filename_ptr, char quote_char)
{
	char	temp_filename[256];
	static int	heredoc_counter = 0;

	ft_strlcpy(temp_filename, "/tmp/heredoc_", sizeof(temp_filename));
	char *pid_str = ft_itoa(getpid());
	ft_strlcat(temp_filename, pid_str, sizeof(temp_filename));
	free(pid_str);

	char *counter_str = ft_itoa(heredoc_counter++);
	ft_strlcat(temp_filename, "_", sizeof(temp_filename));
	ft_strlcat(temp_filename, counter_str, sizeof(temp_filename));
	free(counter_str);

	*temp_filename_ptr = ft_strdup(temp_filename);
	if (!*temp_filename_ptr)
		return (-1);

	int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd < 0)
	{
		perror("minishell: heredoc temp file creation failed");
		return (-1);
	}
	int expand_vars = (quote_char != '\'' && quote_char != '"');
	char *clean_delimiter = ft_strdup(delimiter);
	if (!clean_delimiter)
	{
		close(temp_fd);
		unlink(temp_filename);
		return (-1);
	}
	struct sigaction sa, old_sa;
	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = handle_heredoc_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, &old_sa);
	char *line = NULL;
	size_t line_size = 0;
	ssize_t read_size;

	while (!check_sigint_received())
	{
		write(1, "> ", 2);

		read_size = ft_getline(&line, &line_size, 0);
		if (read_size < 0 || check_sigint_received())
		{
			free(line);
			mini->exit_status = 130;
			close(temp_fd);
			unlink(temp_filename);
			break;
		}
		if (line[read_size - 1] == '\n')
			line[read_size - 1] = '\0';
		if (ft_strcmp(line, clean_delimiter) == 0)
		{
			free(line);
			break;
		}
		char *expanded_line = expand_heredoc_line(mini, line, expand_vars);
		if (expanded_line)
		{
			write(temp_fd, expanded_line, ft_strlen(expanded_line));
			write(temp_fd, "\n", 1);
			free(expanded_line);
		}
		free(line);
		line = NULL;
	}
	close(temp_fd);
	free(clean_delimiter);
	sigaction(SIGINT, &old_sa, NULL);
	if (check_sigint_received())
		return (-1);
	return open(temp_filename, O_RDONLY);
}

int handle_heredoc(t_minishell *mini, t_cmd *cmd)
{
    if (cmd->in_type != HERE_DOC)
        return 0;
    if (cmd->heredoc_list)
        return process_multiple_heredocs(mini, cmd);
    if (!cmd->input_file_name)
        return 0;
    int heredoc_fd = create_heredoc_temp_file_with_quote
    (mini, cmd->input_file_name, &cmd->heredoc_temp_file, cmd->input_quote);
    if (heredoc_fd < 0)
    {
        mini->exit_status = 1;
        return -1;
    }
    cmd->heredoc_fd = heredoc_fd;
    cmd->in_type = REDIR_IN;
    return 0;
}
*/
