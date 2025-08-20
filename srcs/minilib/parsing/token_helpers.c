/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/20 13:03:14 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_redirection(t_minishell *mini, t_tokenize_data *data, char c)
{
	if (c == '>' && data->i + 1 < ft_strlen(mini->promp_input)
		&& mini->promp_input[data->i + 1] == '>')
	{
		append_token(&data->head, &data->tail,
			new_token(">>", REDIR_APPEND, 0));
		data->i += 2;
	}
	else if (c == '<' && data->i + 1 < ft_strlen(mini->promp_input)
		&& mini->promp_input[data->i + 1] == '<')
	{
		append_token(&data->head, &data->tail, new_token("<<", HERE_DOC, 0));
		data->i += 2;
	}
	else if (c == '>')
	{
		append_token(&data->head, &data->tail, new_token(">", REDIR_OUT, 0));
		data->i++;
	}
	else if (c == '<')
	{
		append_token(&data->head, &data->tail, new_token("<", REDIR_IN, 0));
		data->i++;
	}
	return (1);
}

int	finalize_token(t_tokenize_data *data)
{
	if (data->buf_i > 0 || (data->overall_quote != 0 && data->current_quote == 0))
	{
		data->buf[data->buf_i] = '\0';
		append_token(&data->head, &data->tail,
			new_token(data->buf, WORD, data->overall_quote));
		data->buf_i = 0;
		data->overall_quote = 0;
	}
	return (1);
}

t_token	*new_token(const char *value, t_token_type type, char quote)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = ft_strdup(value);
	if (!tok->value)
	{
		free(tok);
		return (NULL);
	}
	tok->type = type;
	tok->quote = quote;
	tok->next = NULL;
	return (tok);
}

void	append_token(t_token **head, t_token **tail, t_token *tok)
{
	if (!*head)
	{
		*head = tok;
		*tail = tok;
	}
	else
	{
		(*tail)->next = tok;
		*tail = tok;
	}
}
