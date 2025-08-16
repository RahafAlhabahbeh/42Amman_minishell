/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmds.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 11:10:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 11:10:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

static void	free_cmd_files(t_cmd *cmd)
{
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
}

static void	free_cmd_fds(t_cmd *cmd)
{
	if (cmd->argv)
	{
		free_argv(cmd->argv);
		cmd->argv = NULL;
	}
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

static void	free_cmd_lists(t_cmd *cmd)
{
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
}

void	free_cmds_array(t_cmd *cmd_array, int count)
{
	int	i;

	if (!cmd_array)
		return ;
	i = 0;
	while (i <= count)
	{
		free_cmd_lists(&cmd_array[i]);
		free_cmd_files(&cmd_array[i]);
		free_cmd_fds(&cmd_array[i]);
		i++;
	}
	free(cmd_array);
}
