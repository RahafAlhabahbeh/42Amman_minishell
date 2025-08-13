/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
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
