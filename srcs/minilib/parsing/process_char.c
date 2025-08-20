/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/20 10:59:05 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_char_escape(t_minishell *mini, t_tokenize_data *data)
{
	char	c;

	c = mini->promp_input[data->i];
	if (c == '\\' && data->i + 1 < data->len && !data->current_quote)
		return (handle_escape_char(mini, &data->i, data->buf, &data->buf_i));
	else if (c == '\\' && data->i + 1 < data->len
		&& data->current_quote == '"')
		return (handle_escape_in_quotes(mini, &data->i, data->buf,
				&data->buf_i));
	else if (c == '\\' && data->current_quote == '\'')
	{
		data->buf[(data->buf_i)++] = c;
		data->i++;
		return (1);
	}
	return (0);
}

static int	handle_special_chars(t_minishell *mini, t_tokenize_data *data,
		char c)
{
	if (handle_quotes(mini, &data->i, &c, &data->current_quote))
	{
		if (data->current_quote != 0 && data->overall_quote == 0)
			data->overall_quote = data->current_quote;
		return (1);
	}
	if (!data->current_quote && (c == '>' || c == '<'))
	{
		finalize_token(data);
		return (handle_redirection(mini, data, c));
	}
	return (0);
}

int	process_char(t_minishell *mini, t_tokenize_data *data)
{
	char	c;

	c = mini->promp_input[data->i];
	if (handle_char_escape(mini, data))
		return (1);
	if (handle_special_chars(mini, data, c))
		return (1);
	if (c == '|' && !data->current_quote)
	{
		finalize_token(data);
		append_token(&data->head, &data->tail, new_token("|", PIPE, 0));
		data->i++;
		return (1);
	}
	if (ft_isspace((unsigned char)c) && !data->current_quote)
	{
		finalize_token(data);
		data->i++;
		return (1);
	}
	data->buf[(data->buf_i)++] = c;
	data->i++;
	return (1);
}
