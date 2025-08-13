/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:52:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	safe_pipe(int pipe_fds[2])
{
	if (pipe(pipe_fds) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
}

void	count_pipe(t_minishell *minishell)
{
	int		count;
	t_token	*cur;

	count = 0;
	cur = minishell->token;
	while (cur)
	{
		if (cur->type == PIPE)
			count++;
		cur = cur->next;
	}
	minishell->pipex_count = count;
}

static void	create_pipes(int *pipefds, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		if (pipe(pipefds + i * 2) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		i++;
	}
}

static void	setup_child_pipes(int *pipefds, int i, int n)
{
	if (i != 0)
	{
		if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
	if (i != n - 1)
	{
		if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit(EXIT_FAILURE);
		}
	}
}

static void	close_all_pipes(int *pipefds, int n)
{
	int	j;

	j = 0;
	while (j < 2 * (n - 1))
	{
		close(pipefds[j]);
		j++;
	}
}

static void	handle_file_redirection(t_cmd *cmd)
{
	int	fd_in;
	int	fd_out;

	if (cmd->input_file_name)
	{
		fd_in = open(cmd->input_file_name, O_RDONLY);
		if (fd_in == -1)
		{
			perror("open input file");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2 input");
			exit(EXIT_FAILURE);
		}
		close(fd_in);
	}
	if (cmd->output_file_name)
	{
		fd_out = open(cmd->output_file_name, 
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd_out == -1)
		{
			perror("open output file");
			exit(EXIT_FAILURE);
		}
		if (dup2(fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2 output");
			exit(EXIT_FAILURE);
		}
		close(fd_out);
	}
}

typedef struct s_pipe_data
{
	int		*pipefds;
	int		i;
	int		n;
	char	**envp;
}	t_pipe_data;

static void	execute_child_command(t_minishell *minishell, t_pipe_data *data)
{
	char	*path;
	int		status;

	setup_child_pipes(data->pipefds, data->i, data->n);
	close_all_pipes(data->pipefds, data->n);
	handle_file_redirection(&minishell->cmd[data->i]);
	if (!minishell->cmd[data->i].argv || !minishell->cmd[data->i].argv[0] || 
		minishell->cmd[data->i].argv[0][0] == '\0')
	{
		write(2, ": command not found\n", 20);
		cleanup_child_process(minishell);
		exit(127);
	}
	status = resolve_cmd_path_with_status(minishell->cmd[data->i].argv[0], 
		minishell, &path);
	if (status != 0)
	{
		if (status == 126)
		{
			if (is_directory(minishell->cmd[data->i].argv[0]))
			{
				write(2, minishell->cmd[data->i].argv[0], 
					ft_strlen(minishell->cmd[data->i].argv[0]));
				write(2, ": Is a directory\n", 17);
			}
			else
			{
				write(2, minishell->cmd[data->i].argv[0], 
					ft_strlen(minishell->cmd[data->i].argv[0]));
				write(2, ": Permission denied\n", 20);
			}
		}
		else
		{
			write(2, minishell->cmd[data->i].argv[0], 
				ft_strlen(minishell->cmd[data->i].argv[0]));
			if (minishell->cmd[data->i].argv[0][0] == '/' || 
				(minishell->cmd[data->i].argv[0][0] == '.' && 
				ft_strchr(minishell->cmd[data->i].argv[0], '/')))
				write(2, ": No such file or directory\n", 28);
			else
				write(2, ": command not found\n", 20);
		}
		cleanup_child_process(minishell);
		exit(status);
	}
	execve(path, minishell->cmd[data->i].argv, data->envp);
	perror("execve");
	free(path);
	cleanup_child_process(minishell);
	exit(EXIT_FAILURE);
}

static int	*init_pipes(int n)
{
	int	*pipefds;

	pipefds = malloc(sizeof(int) * 2 * (n - 1));
	if (!pipefds)
		exit(EXIT_FAILURE);
	create_pipes(pipefds, n);
	return (pipefds);
}

static void	fork_and_execute(t_minishell *ms, t_pipe_data *data, int n)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < n)
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			data->i = i;
			execute_child_command(ms, data);
		}
		i++;
	}
}

static void	wait_for_children(int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		wait(NULL);
		i++;
	}
}

void	execute_piped_commands(t_minishell *minishell, char **envp)
{
	int			n;
	int			*pipefds;
	t_pipe_data	data;

	n = minishell->pipex_count + 1;
	pipefds = init_pipes(n);
	data.pipefds = pipefds;
	data.n = n;
	data.envp = envp;
	fork_and_execute(minishell, &data, n);
	close_all_pipes(pipefds, n);
	wait_for_children(n);
	free(pipefds);
}

