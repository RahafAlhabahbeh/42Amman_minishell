/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/16 12:15:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_heredoc_get_cmd(t_minishell *mini, t_token *cur,
	int cmd_index)
{
	const char	*filename;

	if (!cur->next || cur->next->type != WORD)
		return (-1);
	filename = cur->next->value;
	add_heredoc_to_list(&mini->cmd[cmd_index], filename, cur->next->quote);
	free(mini->cmd[cmd_index].input_file_name);
	mini->cmd[cmd_index].input_file_name = ft_strdup(filename);
	mini->cmd[cmd_index].in_type = HERE_DOC;
	mini->cmd[cmd_index].input_quote = cur->next->quote;
	return (0);
}

static int	handle_pipe(t_minishell *mini, int *cmd_index, int *arg_index)
{
	mini->cmd[*cmd_index].argv[*arg_index] = NULL;
	(*cmd_index)++;
	*arg_index = 0;
	return (0);
}

static int	handle_word(t_minishell *mini, t_token *cur,
	int cmd_index, int *arg_index)
{
	mini->cmd[cmd_index].argv[*arg_index] = ft_strdup(cur->value);
	if (!mini->cmd[cmd_index].argv[*arg_index])
		exit(EXIT_FAILURE);
	(*arg_index)++;
	return (0);
}

static int	process_token_get_cmd(t_minishell *mini, t_token **cur,
	int *cmd_index, int *arg_index)
{
	if ((*cur)->type == PIPE)
		return (handle_pipe(mini, cmd_index, arg_index));
	else if ((*cur)->type == REDIR_IN || (*cur)->type == REDIR_OUT
		|| (*cur)->type == REDIR_APPEND)
	{
		if (handle_redir(mini, *cur, *cmd_index) == -1)
			return (-1);
		*cur = (*cur)->next;
	}
	else if ((*cur)->type == HERE_DOC)
	{
		if (handle_heredoc_get_cmd(mini, *cur, *cmd_index) == -1)
			return (-1);
		*cur = (*cur)->next;
	}
	else if ((*cur)->type == WORD)
		return (handle_word(mini, *cur, *cmd_index, arg_index));
	return (0);
}

int	put_token_to_commands(t_minishell *mini)
{
	t_token	*cur;
	int		cmd_index;
	int		arg_index;

	if (init_cmd_argv(mini) == -1)
		exit(EXIT_FAILURE);
	cur = mini->token;
	cmd_index = 0;
	arg_index = 0;
	while (cur)
	{
		if (process_token_get_cmd(mini, &cur, &cmd_index, &arg_index) == -1)
			return (-1);
		cur = cur->next;
	}
	mini->cmd[cmd_index].argv[arg_index] = NULL;
	cmd_index = 0;
	while (cmd_index <= mini->pipex_count)
	{
		merge_equal_sign_tokens(&mini->cmd[cmd_index]);
		cmd_index++;
	}
	return (0);
}
