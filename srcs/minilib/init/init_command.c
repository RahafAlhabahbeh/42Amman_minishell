/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/13 18:19:13 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	init_commands(t_minishell *minishell)
{
	int	i;

	i = 0;
	while (i < minishell->pipex_count + 1)
	{
		minishell->cmd[i].argv = NULL;
		minishell->cmd[i].in_type = -1;
		minishell->cmd[i].input_file_name = NULL;
		minishell->cmd[i].out_type = -1;
		minishell->cmd[i].output_file_name = NULL;
		minishell->cmd[i].original_stdin = -1;
		minishell->cmd[i].original_stdout = -1;
		if (i < minishell->pipex_count)
			minishell->cmd[i].next = &minishell->cmd[i + 1];
		else
			minishell->cmd[i].next = NULL;
		i++;
	}
}

static void	init_cmd_values(t_minishell *mini, int i)
{
	mini->cmd[i].argv = NULL;
	mini->cmd[i].input_file_name = NULL;
	mini->cmd[i].output_file_name = NULL;
	mini->cmd[i].in_type = -1;
	mini->cmd[i].out_type = -1;
	mini->cmd[i].input_quote = 0;
	mini->cmd[i].heredoc_temp_file = NULL;
	mini->cmd[i].heredoc_fd = -1;
	mini->cmd[i].heredoc_list = NULL;
}

static void	set_cmd_links(t_minishell *mini, int i)
{
	if (i < mini->pipex_count)
		mini->cmd[i].next = &mini->cmd[i + 1];
	else
		mini->cmd[i].next = NULL;
	if (i > 0)
		mini->cmd[i].prev = &mini->cmd[i - 1];
	else
		mini->cmd[i].prev = NULL;
}

void	init_cmd(t_minishell *mini)
{
	int	i;

	mini->cmd = malloc(sizeof(t_cmd) * (mini->pipex_count + 1));
	if (!mini->cmd)
		exit(EXIT_FAILURE);
	i = 0;
	while (i <= mini->pipex_count)
	{
		init_cmd_values(mini, i);
		set_cmd_links(mini, i);
		i++;
	}
}
