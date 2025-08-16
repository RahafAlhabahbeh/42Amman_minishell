/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens_original.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/01/19 00:00:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static t_token	*new_token(const char *value, t_token_type type, char quote)
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

static void	append_token(t_token **head, t_token **tail, t_token *tok)
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

static int	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

static int	handle_quotes(char c, char *current_quote, char *token_quote,
	int buf_i)
{
	if ((c == '\'' || c == '"') && (!*current_quote || *current_quote == c))
	{
		if (!*current_quote)
		{
			*current_quote = c;
			if (buf_i == 0)
				*token_quote = c;
		}
		else
			*current_quote = 0;
		return (1);
	}
	return (0);
}

static void	process_quoted_buffer(char *buf, int *buf_i, char *token_quote,
	t_token **head, t_token **tail)
{
	if (*buf_i > 0)
	{
		buf[*buf_i] = '\0';
		append_token(head, tail, new_token(buf, WORD, *token_quote));
		*buf_i = 0;
		*token_quote = 0;
	}
}

static int	handle_redirections(t_minishell *ms, size_t *i, t_token **head,
	t_token **tail)
{
	if (ms->promp_input[*i] == '>' && ms->promp_input[*i + 1] == '>')
	{
		append_token(head, tail, new_token(">>", REDIR_APPEND, 0));
		*i += 2;
	}
	else if (ms->promp_input[*i] == '<' && ms->promp_input[*i + 1] == '<')
	{
		append_token(head, tail, new_token("<<", HERE_DOC, 0));
		*i += 2;
	}
	else if (ms->promp_input[*i] == '>')
	{
		append_token(head, tail, new_token(">", REDIR_OUT, 0));
		(*i)++;
	}
	else if (ms->promp_input[*i] == '<')
	{
		append_token(head, tail, new_token("<", REDIR_IN, 0));
		(*i)++;
	}
	return (1);
}

static void	finalize_buffer(char *buf, int *buf_i, char *token_quote,
	t_token **head, t_token **tail)
{
	if (*buf_i > 0)
	{
		buf[*buf_i] = '\0';
		append_token(head, tail, new_token(buf, WORD, *token_quote));
		*buf_i = 0;
		*token_quote = 0;
	}
}

static int	process_main_loop(t_minishell *ms, size_t *i, char *buf,
	int *buf_i)
{
	char		c;
	char		current_quote;
	char		token_quote;
	t_token		*head;
	t_token		*tail;

	current_quote = 0;
	token_quote = 0;
	head = NULL;
	tail = NULL;
	while (*i < ft_strlen(ms->promp_input))
	{
		c = ms->promp_input[*i];
		if (handle_quotes(c, &current_quote, &token_quote, *buf_i))
		{
			if (!current_quote && *buf_i == 0)
				append_token(&head, &tail, new_token("", WORD, c));
			else if (!current_quote)
				process_quoted_buffer(buf, buf_i, &token_quote, &head, &tail);
			(*i)++;
			continue ;
		}
		if (!current_quote && (c == '>' || c == '<'))
		{
			finalize_buffer(buf, buf_i, &token_quote, &head, &tail);
			handle_redirections(ms, i, &head, &tail);
		}
		else if (c == '|' && !current_quote)
		{
			finalize_buffer(buf, buf_i, &token_quote, &head, &tail);
			append_token(&head, &tail, new_token("|", PIPE, 0));
			(*i)++;
		}
		else if (ft_isspace((unsigned char)c) && !current_quote)
		{
			finalize_buffer(buf, buf_i, &token_quote, &head, &tail);
			(*i)++;
		}
		else
		{
			buf[(*buf_i)++] = c;
			(*i)++;
		}
	}
	return (current_quote);
}

t_token	*tokenize(t_minishell *minishell)
{
	size_t		i;
	char		buf[1024];
	int			buf_i;
	t_token		*head;
	t_token		*tail;
	int			quote_status;

	i = 0;
	buf_i = 0;
	head = NULL;
	tail = NULL;
	quote_status = process_main_loop(minishell, &i, buf, &buf_i);
	if (buf_i > 0)
	{
		buf[buf_i] = '\0';
		append_token(&head, &tail, new_token(buf, WORD, 0));
	}
	if (quote_status != 0)
	{
		write(2, "Syntax error: unmatched quote\n", 30);
		free_tokens(head);
		return (NULL);
	}
	return (head);
}