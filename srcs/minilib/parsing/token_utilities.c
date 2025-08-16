/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/16 12:40:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	free_token_list(t_token *list)
{
	t_token	*tmp;

	while (list)
	{
		tmp = list;
		list = list->next;
		free(tmp->value);
		free(tmp);
	}
}

t_token	*create_token(char *val, t_token_type type, char quote)
{
	t_token	*tok;

	tok = malloc(sizeof(t_token));
	if (!tok)
		return (NULL);
	tok->value = val;
	tok->type = type;
	tok->quote = quote;
	tok->next = NULL;
	return (tok);
}

static int	add_word_token(t_token **list, t_token **tail,
			char *word, t_token_type type)
{
	t_token	*tok;

	tok = create_token(ft_strdup(word), type, 0);
	if (!tok || !tok->value)
		return (-1);
	if (!*list)
		*list = tok;
	else
		(*tail)->next = tok;
	*tail = tok;
	return (0);
}

int	add_split_tokens(t_token **list, t_token **tail,
	char *str, t_token_type type)
{
	char	**words;
	int		i;
	int		ret;

	words = ft_split(str, ' ');
	if (!words)
		return (-1);
	i = 0;
	while (words[i])
	{
		ret = add_word_token(list, tail, words[i], type);
		free(words[i]);
		if (ret == -1)
		{
			free(words);
			return (-1);
		}
		i++;
	}
	free(words);
	return (0);
}

int	process_token(t_minishell *mini, t_token *cur,
		t_token **list, t_token **tail)
{
	char	*expanded;

	expanded = replace_var(mini, cur->value, cur->quote);
	if (!expanded)
		return (-1);
	if (!cur->quote && ft_strchr(expanded, ' '))
	{
		if (add_split_tokens(list, tail, expanded, cur->type) == -1)
		{
			free(expanded);
			return (-1);
		}
		free(expanded);
	}
	else
	{
		*tail = create_token(expanded, cur->type, cur->quote);
		if (!*tail)
			return (-1);
		if (!*list)
			*list = *tail;
		else
			(*tail)->next = *tail;
	}
	return (0);
}
