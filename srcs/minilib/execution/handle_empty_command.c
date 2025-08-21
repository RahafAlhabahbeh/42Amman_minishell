/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_empty_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 15:44:49 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	read_file_to_stdout(int input_fd)
{
	char	buffer[1024];
	ssize_t	bytes_read;

	bytes_read = read(input_fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(input_fd, buffer, sizeof(buffer));
	}
}

static void	handle_empty_input_redirection(t_minishell *mini, t_cmd *cmd,
	char **child_env)
{
	int		input_fd;

	if (!cmd->input_file_name)
	{
		free_env_array_2(child_env);
		cleanup_child_process(mini);
		exit(0);
	}
	input_fd = open(cmd->input_file_name, O_RDONLY);
	if (input_fd < 0)
	{
		free_env_array_2(child_env);
		cleanup_child_process(mini);
		exit(0);
	}
	read_file_to_stdout(input_fd);
	close(input_fd);
	free_env_array_2(child_env);
	cleanup_child_process(mini);
	exit(0);
}

static void	handle_command_not_found(t_cmd *cmd, char **child_env,
	t_minishell *mini)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd->argv && cmd->argv[0])
		ft_putstr_fd(cmd->argv[0], 2);
	else
		ft_putstr_fd("command", 2);
	ft_putstr_fd(": command not found\n", 2);
	free_env_array_2(child_env);
	cleanup_child_process(mini);
	exit(127);
}

static void	handle_empty_redirections_cases(t_minishell *mini, t_cmd *cmd,
	char **child_env)
{
	if (cmd->in_type == REDIR_IN && mini->pipex_count > 0)
	{
		handle_empty_input_redirection(mini, cmd, child_env);
	}
	else if (cmd->in_type == REDIR_IN || cmd->out_type == REDIR_OUT
		|| cmd->out_type == REDIR_APPEND
		|| cmd->in_type == HERE_DOC || cmd->redirections)
	{
		free_env_array_2(child_env);
		cleanup_child_process(mini);
		exit(0);
	}
	else
	{
		handle_command_not_found(cmd, child_env, mini);
	}
}

void	handle_empty_command(t_minishell *mini, t_cmd *cmd, char **child_env)
{
	if (!cmd->argv || !cmd->argv[0] || cmd->argv[0][0] == '\0')
	{
		handle_empty_redirections_cases(mini, cmd, child_env);
	}
}
