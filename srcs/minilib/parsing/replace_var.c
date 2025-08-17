/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/17 22:13:04 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	process_string_expansion(t_minishell *mini, t_expand_context *ctx)
{
	while (ctx->str[*(ctx->i)])
	{
		if (ctx->str[*(ctx->i)] == '\x01' && ctx->str[*(ctx->i) + 1] == '$')
		{
			if (handle_dollar_escape(ctx) == -1)
				return (-1);
		}
		else if (ctx->str[*(ctx->i)] == '$' && ctx->str[*(ctx->i) + 1])
		{
			if (process_dollar_expansion(mini, ctx) == -1)
				return (-1);
		}
		else
		{
			if (handle_regular_char(ctx) == -1)
				return (-1);
		}
	}
	return (0);
}

static char	*init_and_process(t_minishell *minishell, char *tilde_expanded)
{
	char				*result;
	t_expand_context	ctx;
	size_t				capacity;
	int					i;
	int					j;

	capacity = 1024;
	result = (char *)malloc(capacity);
	if (!result)
		return (free(tilde_expanded), NULL);
	result[0] = '\0';
	i = 0;
	j = 0;
	ctx = (t_expand_context){&result, &capacity, &i, &j, tilde_expanded};
	if (process_string_expansion(minishell, &ctx) == -1)
		return (free(result), free(tilde_expanded), NULL);
	result[j] = '\0';
	return (result);
}

char	*replace_var(t_minishell *minishell, const char *str, char quote)
{
	char	*tilde_expanded;
	char	*result;

	if (quote == '\'')
		return (ft_strdup(str));
	tilde_expanded = expand_tilde(minishell, str);
	if (!tilde_expanded)
		return (NULL);
	result = init_and_process(minishell, tilde_expanded);
	free(tilde_expanded);
	return (result);
}
