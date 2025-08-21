/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 22:15:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/21 02:30:07 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_status_special(t_minishell *mini, t_expand_context *ctx)
{
	char	*status_str;
	char	*temp;
	size_t	len;

	(*(ctx->i))++;
	status_str = ft_itoa(mini->exit_status);
	if (!status_str)
		return (-1);
	len = ft_strlen(status_str);
	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + len + 1);
	if (!temp)
		return (free(status_str), -1);
	*(ctx->result) = temp;
	ft_strlcpy(*(ctx->result) + *(ctx->j), status_str, len + 1);
	*(ctx->j) += len;
	free(status_str);
	return (1);
}

int	handle_pid_special(t_expand_context *ctx)
{
	char	*pid_str;
	char	*temp;
	size_t	len;

	(*(ctx->i))++;
	pid_str = ft_itoa(1000);
	if (!pid_str)
		return (-1);
	len = ft_strlen(pid_str);
	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + len + 1);
	if (!temp)
		return (free(pid_str), -1);
	*(ctx->result) = temp;
	ft_strlcpy(*(ctx->result) + *(ctx->j), pid_str, len + 1);
	*(ctx->j) += len;
	free(pid_str);
	return (1);
}

int	process_dollar_expansion(t_minishell *mini, t_expand_context *ctx)
{
	int	special_result;

	(*(ctx->i))++;
	special_result = handle_special_var(mini, ctx);
	if (special_result == -1)
		return (-1);
	if (special_result == 0)
	{
		if (handle_variable_expansion(mini, ctx) == -1)
			return (-1);
	}
	return (0);
}

int	handle_special_var(t_minishell *mini, t_expand_context *ctx)
{
	if (ctx->str[*(ctx->i)] == '?')
		return (handle_status_special(mini, ctx));
	else if (ctx->str[*(ctx->i)] == '$')
		return (handle_pid_special(ctx));
	else if (ctx->str[*(ctx->i)] == '0')
	{
		(*(ctx->i))++;
		if (handle_minishell_var(ctx) == -1)
			return (-1);
		return (1);
	}
	return (0);
}

int	handle_env_var(t_minishell *mini, t_expand_context *ctx, char *var)
{
	const char	*val;
	char		*temp;
	size_t		len;

	val = get_value_env(mini, var);
	if (!val)
		val = "";
	len = ft_strlen(val);
	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + len + 1);
	if (!temp)
		return (-1);
	*(ctx->result) = temp;
	ft_strlcat(*(ctx->result) + *(ctx->j), val, len + 1);
	*(ctx->j) += len;
	return (0);
}
