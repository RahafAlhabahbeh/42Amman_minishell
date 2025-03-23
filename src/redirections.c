/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:46 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/23 11:33:13 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

/*
 * Redirect output to a file.
 * If append is 1, it uses ">>", otherwise ">".
 * Opens the file in overwrite mode (`O_TRUNC`)
 * `0644` sets file permissions
 * (owner can read/write, others can read).
 */
void redirect_output(char *filename, int append)
{
    int fd;

    if (append)
        fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else
        fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == -1)
    {
        perror("open");
        return;
    }

    /* Redirect STDOUT to the file */
    dup2(fd, STDOUT_FILENO);
    close(fd); /* Don't forget to close the file descriptor */
}

void	redirect_input(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		write(2, "Error: Failed to open file\n", 27);
		return;
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}

void	heredoc(char *delimiter)
{
	char	*line;
	int		pipe_fd[2];

	if (pipe(pipe_fd) == -1)
		return ((void)write(2, "Error: Pipe failed\n", 19));
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strncmp(line, delimiter, ft_strlen(line)) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}


void	process_redirections(char **cmd)
{
	int	fd;
	int	i;

	i = 0;
	while (cmd[i])
	{
		if (!ft_strncmp(cmd[i], "<", 1) && cmd[i + 1])
			fd = open_file(cmd[++i], O_RDONLY, 0644);
		else if (!ft_strncmp(cmd[i], ">", ft_strlen(cmd[i])) && cmd[i + 1])
			fd = open_file(cmd[++i], O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (!ft_strncmp(cmd[i], ">>", ft_strlen(cmd[i])) && cmd[i + 1])
			fd = open_file(cmd[++i], O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd != -1)
			dup2(fd, (!ft_strncmp(cmd[i - 1], "<", ft_strlen(cmd[i]))) ? STDIN_FILENO : STDOUT_FILENO);
		if (fd > 0)
			close(fd);
		i++;
	}
}

void	execute_with_redirections(char **cmd, char **envp)
{
	process_redirections(cmd);
	execve(cmd[0], cmd, envp);
	perror("execve");
	exit(1);
}
