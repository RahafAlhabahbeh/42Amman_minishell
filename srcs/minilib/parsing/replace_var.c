/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_special_var(t_minishell *mini, char **result,
		size_t *capacity, int *j, const char *str, int *i)
{
	char	*status_str;
	char	*pid_str;
	char	*temp;
	size_t	len;

	if (str[*i] == '?')
	{
		(*i)++;
		status_str = ft_itoa(mini->exit_status);
		if (!status_str)
			return (-1);
		len = ft_strlen(status_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
			return (free(status_str), -1);
		*result = temp;
		ft_strlcpy(*result + *j, status_str, len + 1);
		*j += len;
		free(status_str);
		return (1);
	}
	else if (str[*i] == '$')
	{
		(*i)++;
		pid_str = ft_itoa(getpid());
		if (!pid_str)
			return (-1);
		len = ft_strlen(pid_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
			return (free(pid_str), -1);
		*result = temp;
		ft_strlcpy(*result + *j, pid_str, len + 1);
		*j += len;
		free(pid_str);
		return (1);
	}
	return (0);
}

static int	handle_variable_expansion(t_minishell *mini, const char *str,
		char **result, size_t *capacity, int *i, int *j)
{
	char	var[256];
	int		is_braced;
	int		var_len;
	const char	*val;
	char	*temp;
	size_t	len;
	char	*status_str;
	char	*pid_str;

	ft_memset(var, 0, sizeof(var));
	var_len = extract_var_name(str, i, var, &is_braced);
	if (var_len < 0 || var_len == 0)
	{
		temp = safe_resize_buffer(*result, capacity, *j + 2);
		if (!temp)
			return (-1);
		*result = temp;
		(*result)[(*j)++] = '$';
		(*result)[*j] = '\0';
		return (0);
	}
	if (var_len == 1 && var[0] == '?')
	{
		status_str = ft_itoa(mini->exit_status);
		if (!status_str)
			return (-1);
		len = ft_strlen(status_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
		{
			free(status_str);
			return (-1);
		}
		*result = temp;
		ft_strlcat(*result + *j, status_str, len + 1);
		*j += len;
		free(status_str);
		return (0);
	}
	if (var_len == 1 && var[0] == '$')
	{
		pid_str = ft_itoa(getpid());
		if (!pid_str)
			return (-1);
		len = ft_strlen(pid_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
		{
			free(pid_str);
			return (-1);
		}
		*result = temp;
		ft_strlcat(*result + *j, pid_str, len + 1);
		*j += len;
		free(pid_str);
		return (0);
	}
	val = get_value_env(mini, var);
	if (!val)
		val = "";
	len = ft_strlen(val);
	temp = safe_resize_buffer(*result, capacity, *j + len + 1);
	if (!temp)
		return (-1);
	*result = temp;
	ft_strlcat(*result + *j, val, len + 1);
	*j += len;
	return (0);
}

static int	process_string_expansion(t_minishell *mini, const char *str,
					char **result, size_t *capacity, int *i, int *j)
{
	char	*temp;
	int		special_result;

	while (str[*i])
	{
		if (str[*i] == '\x01' && str[*i + 1] == '$')
		{
			temp = safe_resize_buffer(*result, capacity, *j + 2);
			if (!temp)
				return (-1);
			*result = temp;
			(*result)[(*j)++] = '$';
			(*result)[*j] = '\0';
			*i += 2;
		}
		else if (str[*i] == '$' && str[*i + 1])
		{
			(*i)++;
			special_result = handle_special_var(mini, result, capacity, j, str, i);
			if (special_result == -1)
				return (-1);
			if (special_result == 0)
			{
				if (handle_variable_expansion(mini, str, result, capacity, i, j)
					== -1)
					return (-1);
			}
		}
		else
		{
			temp = safe_resize_buffer(*result, capacity, *j + 2);
			if (!temp)
				return (-1);
			*result = temp;
			(*result)[(*j)++] = str[(*i)++];
			(*result)[*j] = '\0';
		}
	}
	return (0);
}

char	*replace_var(t_minishell *minishell, const char *str, char quote)
{
	size_t	capacity;
	char	*result;
	int		i;
	int		j;
	char	*tilde_expanded;

	if (quote == '\'')
		return (ft_strdup(str));
	tilde_expanded = expand_tilde(minishell, str);
	if (!tilde_expanded)
		return (NULL);
	capacity = 1024;
	result = (char *)malloc(capacity);
	if (!result)
	{
		free(tilde_expanded);
		return (NULL);
	}
	result[0] = '\0';
	i = 0;
	j = 0;
	if (process_string_expansion(minishell, tilde_expanded,
		&result, &capacity, &i, &j) == -1)
	{
		free(result);
		free(tilde_expanded);
		return (NULL);
	}
	result[j] = '\0';
	free(tilde_expanded);
	return (result);
}
