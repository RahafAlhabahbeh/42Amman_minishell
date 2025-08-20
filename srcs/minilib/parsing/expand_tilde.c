/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_tilde.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:52:23 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static const char	*get_home_path(t_minishell *mini, char **to_free)
{
	const char	*home;

	*to_free = NULL;
	home = get_value_env(mini, "HOME");
	if (!home)
	{
		*to_free = get_user_home_dir();
		home = *to_free;
		if (!home)
		{
			*to_free = NULL;
			home = "";
		}
	}
	return (home);
}

static char	*join_home_and_rest(const char *home,
	const char *rest, char *to_free)
{
	size_t	home_len;
	size_t	rest_len;
	char	*result;

	home_len = ft_strlen(home);
	rest_len = ft_strlen(rest);
	result = (char *)malloc(home_len + rest_len + 1);
	if (!result)
	{
		if (to_free)
			free(to_free);
		return (NULL);
	}
	ft_memcpy(result, home, home_len);
	ft_memcpy(result + home_len, rest, rest_len);
	result[home_len + rest_len] = '\0';
	if (to_free)
		free(to_free);
	return (result);
}

char	*expand_tilde(t_minishell *mini, const char *str)
{
	char	*home_to_free;
	const char	*home;

	home_to_free = NULL;
	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_home_path(mini, &home_to_free);
		return (join_home_and_rest(home, str + 1, home_to_free));
	}
	return (ft_strdup(str));
}

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
/*
char	*expand_tilde(t_minishell *minishell, const char *str)
{
	const char	*home;
	char		*home_to_free;
	size_t		home_len;
	size_t		rest_len;
	char		*result;

	home_to_free = NULL;
	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_value_env(minishell, "HOME");
		if (!home)
		{
			home_to_free = get_user_home_dir();
			home = home_to_free;
			if (!home)
				home = "";
		}
		home_len = ft_strlen(home);
		rest_len = ft_strlen(str + 1);
		result = (char *)malloc(home_len + rest_len + 1);
		if (!result)
		{
			if (home_to_free)
				free(home_to_free);
			return (NULL);
		}
		ft_memcpy(result, home, home_len);
		ft_memcpy(result + home_len, str + 1, rest_len);
		result[home_len + rest_len] = '\0';
		if (home_to_free)
			free(home_to_free);
		return (result);
	}
	return (ft_strdup(str));
}*/
