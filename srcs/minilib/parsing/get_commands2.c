/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:30:57 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"
#define MAX_ARGS 512

int	init_cmd_argv(t_minishell *mini)
{
	int	i;
	int	j;

	i = 0;
	while (i <= mini->pipex_count)
	{
		mini->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
		if (!mini->cmd[i].argv)
			return (-1);
		j = 0;
		while (j < MAX_ARGS)
		{
			mini->cmd[i].argv[j] = NULL;
			j++;
		}
		i++;
	}
	return (0);
}

void	set_redirection(t_minishell *mini, int cmd_index,
	const char *filename, t_token_type type)
{
	if (type == REDIR_IN)
	{
		// For input redirections, only set if not already set
		// This ensures we process redirections from left to right
		if (!mini->cmd[cmd_index].input_file_name)
		{
			mini->cmd[cmd_index].input_file_name = ft_strdup(filename);
			mini->cmd[cmd_index].in_type = REDIR_IN;
		}
	}
	if (type == REDIR_OUT)
	{
		// Create the file even if it will be overwritten
		int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (fd >= 0)
			close(fd);
		
		free(mini->cmd[cmd_index].output_file_name);
		mini->cmd[cmd_index].output_file_name = ft_strdup(filename);
		mini->cmd[cmd_index].out_type = REDIR_OUT;
	}
	if (type == REDIR_APPEND)
	{
		// Create the file even if it will be overwritten
		int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd >= 0)
			close(fd);
		
		free(mini->cmd[cmd_index].output_file_name);
		mini->cmd[cmd_index].output_file_name = ft_strdup(filename);
		mini->cmd[cmd_index].out_type = REDIR_APPEND;
	}
}

int	handle_redir(t_minishell *mini, t_token *cur, int cmd_index)
{
	const char	*filename;

	if (!cur->next || cur->next->type != WORD)
		return (-1);
	filename = cur->next->value;
	// Don't check file existence at parsing time, let execution handle it
	set_redirection(mini, cmd_index, filename, cur->type);
	return (0);
}
