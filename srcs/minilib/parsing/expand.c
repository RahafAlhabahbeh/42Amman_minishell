/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 16:07:32 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*expand(t_minishell *mini)
{
	t_token	*cur;
	t_token	*new_list;
	t_token	*tail;

	cur = mini->token;
	new_list = NULL;
	tail = NULL;
	while (cur)
	{
		if (process_token(mini, cur, &new_list, &tail) == -1)
		{
			free_tokens(new_list);
			return (NULL);
		}
		cur = cur->next;
	}
	return (new_list);
}
/*
t_token	*expand(t_minishell *minishell)
{
	t_token	*cur;
	t_token	*new_list;
	t_token	*tail;

	cur = minishell->token;
	new_list = NULL;
	tail = NULL;
	while (cur)
	{
		char	*expanded;

		expanded = replace_var(minishell, cur->value, cur->quote);
		if (!expanded)
		{
			while (new_list)
			{
				t_token	*tmp;

				tmp = new_list;
				new_list = new_list->next;
				free(tmp->value);
				free(tmp);
			}
			return (NULL);
		}
		if (!cur->quote && ft_strchr(expanded, ' '))
		{
			char	**words;
			int		i;

			words = ft_split(expanded, ' ');
			free(expanded);
			if (!words)
			{
				while (new_list)
				{
					t_token *tmp = new_list;
					new_list = new_list->next;
					free(tmp->value);
					free(tmp);
				}
				return NULL;
			}
			i = 0;
			while (words[i])
			{
				t_token	*new_tok;

				new_tok = malloc(sizeof(t_token));
				if (!new_tok)
				{
					for (int j = i; words[j]; j++)
						free(words[j]);
					free(words);
					while (new_list)
					{
						t_token *tmp = new_list;
						new_list = new_list->next;
						free(tmp->value);
						free(tmp);
					}
					return NULL;
				}
				new_tok->value = ft_strdup(words[i]);
				new_tok->type = cur->type;
				new_tok->quote = 0;
				new_tok->next = NULL;
				if (!new_tok->value)
				{
					free(new_tok);
					for (int j = i; words[j]; j++)
						free(words[j]);
					free(words);
					while (new_list)
					{
						t_token *tmp = new_list;
						new_list = new_list->next;
						free(tmp->value);
						free(tmp);
					}
					return NULL;
				}
				if (!new_list)
					new_list = new_tok;
				else
					tail->next = new_tok;
				tail = new_tok;
				free(words[i]);
				i++;
			}
			free(words);
		}
		else
		{
			t_token	*new_tok;

			new_tok = malloc(sizeof(t_token));
			if (!new_tok)
			{
				free(expanded);
				while (new_list)
				{
					t_token *tmp = new_list;
					new_list = new_list->next;
					free(tmp->value);
					free(tmp);
				}
				return NULL;
			}
			new_tok->value = expanded;
			new_tok->type = cur->type;
			new_tok->quote = cur->quote;
			new_tok->next = NULL;
			if (!new_list)
				new_list = new_tok;
			else
				tail->next = new_tok;
			tail = new_tok;
		}
		cur = cur->next;
	}
	return (new_list);
}
*/
