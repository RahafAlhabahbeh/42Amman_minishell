/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utilities.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 01:03:20 by rahaf            ###   ########.fr       */
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
	tok->value = ft_strdup(val);
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

static int	add_word_token(t_token **list, t_token **tail,
			char *word, t_token_type type)
{
	t_token	*tok;

	tok = create_token(word, type, 0);
	if (!tok)
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
	t_token	*new_token_node;

	expanded = replace_var(mini, cur->value, cur->quote);
	if (!expanded)
		return (-1);
	if (!cur->quote && ft_strchr(expanded, ' '))
	{
		if (add_split_tokens(list, tail, expanded, cur->type) == -1)
			return (free(expanded), -1);
		return (free(expanded), 0);
	}
	new_token_node = create_token(expanded, cur->type, cur->quote);
	free(expanded);
	if (!new_token_node)
		return (-1);
	if (!*list)
		*list = new_token_node;
	else
		(*tail)->next = new_token_node;
	*tail = new_token_node;
	return (0);
}
