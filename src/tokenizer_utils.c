/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 14:06:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/03 16:00:31 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

t_token_type get_token_type(char *str)
{
	if (strcmp(str, "|") == 0)
		return (TOK_PIPE);
	if (strcmp(str, "<<") == 0)
		return (TOK_HEREDOC);
	if (strcmp(str, ">>") == 0)
		return (TOK_APPEND);
	if (strcmp(str, "<") == 0)
		return (TOK_RED_IN);
	if (strcmp(str, ">") == 0)
		return (TOK_RED_OUT);
	return (TOK_WORD);
}
void    token_add_back(t_token **lst, t_token *new)
{
	t_token	*cur;

	if (!lst || !new)
		return ;
	if (!*lst)
		*lst = new;
	else
	{
		cur = *lst;
		while (cur->next)
			cur = cur->next;
		cur->next = new;
	}
}
void	free_tokens(t_token *head)
{
	t_token	*tmp;

	while (head)
	{
		tmp = head->next;
		free(head->value);
		free(head);
		head = tmp;
	}
}
void	free_split(char **parts)
{
	int	i;

	if (!parts)
		return;
	i = 0;
	while (parts[i])
	{
		free(parts[i]);
		i++;
	}
	free(parts);
}

