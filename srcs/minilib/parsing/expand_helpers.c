/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 22:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 00:39:29 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	handle_exit_status(t_minishell *mini, t_expand_context *ctx)
{
	char	*status_str;
	char	*temp;
	size_t	len;

	status_str = ft_itoa(mini->exit_status);
	if (!status_str)
		return (-1);
	len = ft_strlen(status_str);
	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + len + 1);
	if (!temp)
		return (free(status_str), -1);
	*(ctx->result) = temp;
	ft_strlcat(*(ctx->result) + *(ctx->j), status_str, len + 1);
	*(ctx->j) += len;
	free(status_str);
	return (0);
}

int	handle_pid_var(t_expand_context *ctx)
{
	char	*pid_str;
	char	*temp;
	size_t	len;

	pid_str = ft_itoa(getpid());
	if (!pid_str)
		return (-1);
	len = ft_strlen(pid_str);
	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + len + 1);
	if (!temp)
		return (free(pid_str), -1);
	*(ctx->result) = temp;
	ft_strlcat(*(ctx->result) + *(ctx->j), pid_str, len + 1);
	*(ctx->j) += len;
	free(pid_str);
	return (0);
}

int	handle_minishell_var(t_expand_context *ctx)
{
	char	*temp;

	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + 12);
	if (!temp)
		return (-1);
	*(ctx->result) = temp;
	ft_strlcat(*(ctx->result) + *(ctx->j), "./minishell", 12);
	*(ctx->j) += 11;
	return (0);
}

int	handle_dollar_escape(t_expand_context *ctx)
{
	char	*temp;

	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + 2);
	if (!temp)
		return (-1);
	*(ctx->result) = temp;
	(*(ctx->result))[(*(ctx->j))++] = '$';
	(*(ctx->result))[*(ctx->j)] = '\0';
	*(ctx->i) += 2;
	return (0);
}

int	handle_regular_char(t_expand_context *ctx)
{
	char	*temp;

	temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
			*(ctx->j) + 2);
	if (!temp)
		return (-1);
	*(ctx->result) = temp;
	(*(ctx->result))[(*(ctx->j))++] = ctx->str[(*(ctx->i))++];
	(*(ctx->result))[*(ctx->j)] = '\0';
	return (0);
}

int	handle_variable_expansion(t_minishell *mini, t_expand_context *ctx)
{
	char	var[256];
	int		is_braced;
	int		var_len;
	char	*temp;

	ft_memset(var, 0, sizeof(var));
	var_len = extract_var_name(ctx->str, ctx->i, var, &is_braced);
	if (var_len < 0 || var_len == 0)
	{
		temp = safe_resize_buffer(*(ctx->result), ctx->capacity,
				*(ctx->j) + 2);
		if (!temp)
			return (-1);
		*(ctx->result) = temp;
		(*(ctx->result))[(*(ctx->j))++] = '$';
		(*(ctx->result))[*(ctx->j)] = '\0';
		return (0);
	}
	if (var_len == 1 && var[0] == '?')
		return (handle_exit_status(mini, ctx));
	if (var_len == 1 && var[0] == '$')
		return (handle_pid_var(ctx));
	if (var_len == 1 && var[0] == '0')
		return (handle_minishell_var(ctx));
	return (handle_env_var(mini, ctx, var));
}
