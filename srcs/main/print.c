/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static const char	*get_token_type_string(t_token_type type)
{
	if (type == WORD)
		return ("WORD");
	if (type == PIPE)
		return ("PIPE");
	if (type == REDIR_IN)
		return ("REDIR_IN");
	if (type == REDIR_OUT)
		return ("REDIR_OUT");
	if (type == REDIR_APPEND)
		return ("REDIR_APPEND");
	if (type == HERE_DOC)
		return ("HERE_DOC");
	return ("UNKNOWN");
}

static void	print_token_info(t_token *token)
{
	const char	*type_str;
	char		quote_char;

	type_str = get_token_type_string(token->type);
	quote_char = '-';
	if (token->quote)
		quote_char = token->quote;
	printf("  [%s] \"%s\" (quote: %c)\n", type_str, token->value, quote_char);
}

void	print_tokens(t_token *token)
{
	printf("\n🧩 Token List:\n");
	while (token)
	{
		print_token_info(token);
		token = token->next;
	}
	printf("🔚 End of Tokens\n");
}