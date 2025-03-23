/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 11:42:41 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/23 11:01:09 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void	child_process(int *pipe_fd, char **cmd, char **envp, int in_out)
{
	dup2(pipe_fd[in_out], in_out);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execve(cmd[0], cmd, envp);
	perror("execve");
	exit(1);
}

void execute_pipe(char **cmd1, char **cmd2, char **envp)
{
    int pipe_fd[2];
    pid_t pid1, pid2;

    if (pipe(pipe_fd) == -1 || (pid1 = fork()) == -1 || (pid2 = fork()) == -1)
    {
        perror("pipe/fork");
        return;
    }
    if (pid1 == 0) // First child
    {
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]);
        execve(cmd1[0], cmd1, envp);
        exit(1); // In case execve fails
    }
    if (pid2 == 0) // Second child
    {
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[1]);
        execve(cmd2[0], cmd2, envp);
        exit(1); // In case execve fails
    }
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}


char	*join_path(char *dir, char *cmd)
{
	char	*full_cmd;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	full_cmd = malloc(len);
	if (!full_cmd)
		return (NULL);
	ft_strlcpy(full_cmd, dir, ft_strlen(dir));
	ft_strlcat(full_cmd, "/", ft_strlen(full_cmd));
	ft_strlcat(full_cmd, cmd, ft_strlen(full_cmd));
	return (full_cmd);
}

char	*get_cmd_path(char *cmd, char **paths)
{
	int		i;
	char	*full_cmd;

	if (!cmd || (ft_strchr(cmd, '/') && access(cmd, X_OK) == 0))
		return (ft_strdup(cmd));
	i = -1;
	while (paths[++i])
	{
		full_cmd = join_path(paths[i], cmd);
		if (full_cmd && access(full_cmd, X_OK) == 0)
			return (full_cmd);
		free(full_cmd);
	}
	return (NULL);
}

void	execute_command(char *cmd, char **args, char **envp)
{
	char	**paths;
	char	*cmd_path;

	if (!cmd || !(paths = ft_split(getenv("PATH"), ':')))
		return (perror("Error: PATH not found"), exit(127));
	cmd_path = get_cmd_path(cmd, paths);
	ft_free_split_recursive(paths, 0);
	if (!cmd_path)
		return (perror("Error: command not found"), exit(127));
	execve(cmd_path, args, envp);
	perror("Error: execve failed");
	free(cmd_path);
	exit(126);
}
