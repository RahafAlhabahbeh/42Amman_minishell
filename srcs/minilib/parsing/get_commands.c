/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_commands.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 14:10:58 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_heredoc_get_cmd(t_minishell *mini, t_token *cur,
	int cmd_index)
{
	const char	*delimiter;
	char		*temp_filename;
	int			heredoc_fd;

	if (!cur->next || cur->next->type != WORD)
		return (-1);
	delimiter = cur->next->value;
	cleanup_previous_heredoc(mini, cmd_index);
	if (create_heredoc_temp_file_with_quote(mini, delimiter, &temp_filename,
			cur->next->quote) < 0)
		return (-1);
	heredoc_fd = open(temp_filename, O_RDONLY);
	if (heredoc_fd < 0)
	{
		free(temp_filename);
		return (-1);
	}
	mini->cmd[cmd_index].input_file_name = temp_filename;
	mini->cmd[cmd_index].in_type = HERE_DOC;
	mini->cmd[cmd_index].input_quote = cur->next->quote;
	mini->cmd[cmd_index].heredoc_fd = heredoc_fd;
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
