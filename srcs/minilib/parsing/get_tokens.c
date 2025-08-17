/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 17:22:58 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*tokenize(t_minishell *minishell)
{
	t_tokenize_data	data;

	data.i = 0;
	data.len = ft_strlen(minishell->promp_input);
	data.buf_i = 0;
	data.head = NULL;
	data.tail = NULL;
	data.current_quote = 0;
	data.overall_quote = 0;
	if (!tokenize_main_loop(minishell, &data))
		return (NULL);
	if (data.current_quote != 0)
	{
		write(2, "Syntax error: unmatched quote\n", 30);
		free_tokens(data.head);
		return (NULL);
	}
	return (data.head);
}