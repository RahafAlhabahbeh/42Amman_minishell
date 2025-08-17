/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 21:06:47 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*get_exit_status_str(int status)
{
	char	*result;

	result = ft_itoa(status);
	if (!result)
		return (ft_strdup("0"));
	return (result);
}

static char	*get_var_value(t_minishell *mini, const char *name)
{
	char	*value;

	if (ft_strcmp(name, "?") == 0)
		return (get_exit_status_str(mini->exit_status));
	value = get_value_env(mini, name);
	if (!value)
		return (NULL);
	return (ft_strdup(value));
}

static void	append_var(char *result, int *j, char *var_value)
{
	int	len;
	int	k;

	if (!var_value)
		return ;
	len = ft_strlen(var_value);
	k = 0;
	while (k < len && *j < 1023)
	{
		result[*j] = var_value[k];
		k++;
		(*j)++;
	}
	result[*j] = '\0';
	free(var_value);
}

static char	*process_variable(t_minishell *mini, char *line, int *i)
{
	char	var_name[256];
	int		k;

	(*i)++;
	k = 0;
	if (line[*i] == '?')
	{
		var_name[k++] = line[*i];
		(*i)++;
	}
	else
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			var_name[k++] = line[(*i)++];
	var_name[k] = '\0';
	return (get_var_value(mini, var_name));
}

char	*expand_heredoc_line(t_minishell *mini, char *line, int expand_vars)
{
	char	result[1024];
	char	*var_value;
	int		i;
	int		j;

	if (!line)
		return (ft_strdup(""));
	if (!expand_vars)
		return (ft_strdup(line));
	i = 0;
	j = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1] != '\0'
			&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_'
				|| line[i + 1] == '?'))
		{
			var_value = process_variable(mini, line, &i);
			append_var(result, &j, var_value);
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (ft_strdup(result));
}
