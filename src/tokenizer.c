/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:23:29 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/03 16:01:08 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static t_token *new_token(char *value, t_token_type type) 
{
	t_token	*node;

	node = malloc(sizeof(*node));
	if (!node)
		return (NULL);
	node->value = value;
	node->type = type;
	node->next = NULL;
	return (node);
}

/*
** Split on spaces, categorize each piece, and build a linked list.
*/
t_token	*lexer(char *input)
{
	char	**parts;
	t_token	*head;
	t_token	*tok;
	int		i;

	head = NULL;
	parts = ft_split(input, ' ');
	if (!parts)
		return (NULL);
	i = 0;
	while (parts[i])
	{
		tok = new_token(parts[i], get_token_type(parts[i]));
		if (!tok)
		{
			free_tokens(head);
            free_split(parts);
			return (NULL);
		}
		token_add_back(&head, tok);
		i++;
	}
	free_split(parts);
	return (head);
}