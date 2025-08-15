/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 15:31:43 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		if (head->value)
			free(head->value);
		free(head);
		head = tmp;
	}
}

static void	free_argv(char **argv)
{
	int	j;

	if (!argv)
		return ;
	j = 0;
	while (argv[j])
	{
		free(argv[j]);
		j++;
	}
	free(argv);
}

static void	free_single_cmd(t_cmd *cmd)
{
	if (cmd->argv)
	{
		free_argv(cmd->argv);
		cmd->argv = NULL;
	}
	if (cmd->input_file_name)
	{
		free(cmd->input_file_name);
		cmd->input_file_name = NULL;
	}
	if (cmd->output_file_name)
	{
		free(cmd->output_file_name);
		cmd->output_file_name = NULL;
	}
	if (cmd->heredoc_temp_file)
	{
		unlink(cmd->heredoc_temp_file);
		free(cmd->heredoc_temp_file);
		cmd->heredoc_temp_file = NULL;
	}
	if (cmd->heredoc_fd >= 0)
	{
		close(cmd->heredoc_fd);
		cmd->heredoc_fd = -1;
	}
	if (cmd->heredoc_list)
	{
		free_heredoc_list(cmd->heredoc_list);
		cmd->heredoc_list = NULL;
	}
	// Close any saved original file descriptors to prevent FD leaks
	if (cmd->original_stdin >= 0)
	{
		close(cmd->original_stdin);
		cmd->original_stdin = -1;
	}
	if (cmd->original_stdout >= 0)
	{
		close(cmd->original_stdout);
		cmd->original_stdout = -1;
	}
}

void	free_cmds_array(t_cmd *cmd_array, int count)
{
	int	i;

	if (!cmd_array)
		return ;
	i = 0;
	while (i <= count)
	{
		free_single_cmd(&cmd_array[i]);
		i++;
	}
	free(cmd_array);
}

void	free_commands(t_cmd *cmd, int count)
{
	free_cmds_array(cmd, count);
}

void	free_minishell(t_minishell *mini)
{
	// Clean up heredoc files first before freeing command structures
	cleanup_heredoc_files(mini);
	if (mini->promp_input)
		free(mini->promp_input);
	if (mini->token)
		free_tokens(mini->token);
	if (mini->cmd)
		free_cmds_array(mini->cmd, mini->pipex_count);
	if (mini->env_list)
		free_env_list(mini->env_list);
}

void	free_env_list(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env->next;
		free(env->key);
		free(env->value);
		free(env);
		env = tmp;
	}
}

void	reset_minishell(t_minishell *mini)
{
	// Clean up heredoc files first before freeing command structures
	cleanup_heredoc_files(mini);
	if (mini->promp_input)
	{
		free(mini->promp_input);
		mini->promp_input = NULL;
	}
	if (mini->token)
	{
		free_tokens(mini->token);
		mini->token = NULL;
	}
	if (mini->cmd)
	{
		free_commands(mini->cmd, mini->pipex_count);
		mini->cmd = NULL;
		mini->cmd_count = 0;
	}
	mini->pipex_count = 0;
}

void	cleanup_child_process(t_minishell *mini)
{
	if (mini)
	{
		// Clean up heredoc files first before freeing command structures
		cleanup_heredoc_files(mini);
		if (mini->promp_input)
			free(mini->promp_input);
		if (mini->token)
			free_tokens(mini->token);
		if (mini->cmd)
			free_cmds_array(mini->cmd, mini->pipex_count);
		if (mini->env_list)
			free_env_list(mini->env_list);
	}
}

void	close_unused_fds(int start_fd, int max_fd)
{
	int	fd;

	fd = start_fd;
	while (fd < max_fd)
	{
		close(fd);  // Close will fail silently if fd is not open
		fd++;
	}
}

void	close_extra_fds(int prev_fd, int *pipe_fds, int is_last)
{
	int	fd;

	// Close all file descriptors from 3 to 255, except the ones we need
	fd = 3;
	while (fd < 256)
	{
		// Don't close the file descriptors we actually need
		if (fd == prev_fd)
		{
			fd++;
			continue;
		}
		if (pipe_fds && !is_last && (fd == pipe_fds[0] || fd == pipe_fds[1]))
		{
			fd++;
			continue;
		}
		close(fd);  // Close will fail silently if fd is not open
		fd++;
	}
}

void	free_env_array(char **arr, int count)
{
	while (count > 0)
		free(arr[--count]);
	free(arr);
}
