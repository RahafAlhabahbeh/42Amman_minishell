/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_loop.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 21:06:47 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_escape_char(t_minishell *mini, size_t *i, char *buf, int *buf_i)
{
	char	next_char;

	if (*i + 1 >= ft_strlen(mini->promp_input))
		return (0);
	next_char = mini->promp_input[*i + 1];
	if (next_char == '|')
	{
		buf[(*buf_i)++] = '|';
		*i += 2;
		return (1);
	}
	if (next_char == '<' || next_char == '>' || next_char == '"'
		|| next_char == '\'' || next_char == '\\' || next_char == ' '
		|| next_char == '\t' || next_char == '\n' || next_char == '$')
		return (handle_other_escapes(mini, i, buf, buf_i));
	buf[(*buf_i)++] = mini->promp_input[*i];
	(*i)++;
	return (1);
}

int	handle_other_escapes(t_minishell *mini, size_t *i, char *buf, int *buf_i)
{
	char	next_char;

	if (*i + 1 >= ft_strlen(mini->promp_input))
		return (0);
	next_char = mini->promp_input[*i + 1];
	if (next_char == '<' || next_char == '>' || next_char == '"'
		|| next_char == '\'' || next_char == '\\' || next_char == ' '
		|| next_char == '\t' || next_char == '\n')
	{
		buf[(*buf_i)++] = next_char;
		*i += 2;
		return (1);
	}
	if (next_char == '$')
	{
		buf[(*buf_i)++] = '\x01';
		buf[(*buf_i)++] = '$';
		*i += 2;
		return (1);
	}
	return (0);
}

int	handle_escape_in_quotes(t_minishell *mini, size_t *i, char *buf, int *buf_i)
{
	char	next_char;

	if (*i + 1 >= ft_strlen(mini->promp_input))
		return (0);
	next_char = mini->promp_input[*i + 1];
	if (next_char == '"' || next_char == '\\' || next_char == '$'
		|| next_char == '`')
	{
		if (next_char == '$')
		{
			buf[(*buf_i)++] = '\x01';
			buf[(*buf_i)++] = '$';
		}
		else
			buf[(*buf_i)++] = next_char;
		*i += 2;
		return (1);
	}
	buf[(*buf_i)++] = mini->promp_input[*i];
	(*i)++;
	return (1);
}

int	handle_quotes(t_minishell *mini, size_t *i, char *c, char *current_quote)
{
	(void)mini;
	if ((*c == '\'' || *c == '"') && (!*current_quote || *current_quote == *c))
	{
		if (!*current_quote)
			*current_quote = *c;
		else
			*current_quote = 0;
		(*i)++;
		return (1);
	}
	return (0);
}

int	tokenize_main_loop(t_minishell *mini, t_tokenize_data *data)
{
	while (data->i < data->len)
	{
		if (!process_char(mini, data))
			return (0);
	}
	finalize_token(data);
	return (1);
}
