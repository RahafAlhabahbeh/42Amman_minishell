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

void	free_minishell(t_minishell *mini)
{
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

void	reset_minishell(t_minishell *mini)
{
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
		free_cmds_array(mini->cmd, mini->pipex_count);
		mini->cmd = NULL;
		mini->cmd_count = 0;
	}
	mini->pipex_count = 0;
}
