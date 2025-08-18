/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 15:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*int_to_str(int num)
{
	char	str[16];
	int		i;
	int		start;
	int		end;
	char	tmp;

	i = 0;
	if (num == 0)
		str[i++] = '0';
	while (num > 0)
	{
		str[i++] = (num % 10) + '0';
		num /= 10;
	}
	str[i] = '\0';
	start = 0;
	end = i - 1;
	while (start < end)
	{
		tmp = str[start];
		str[start++] = str[end];
		str[end--] = tmp;
	}
	return (ft_strdup(str));
}

static char	*get_variable_value(t_minishell *mini, char *var_name)
{
	char	*value;

	if (ft_strcmp(var_name, "?") == 0)
		return (int_to_str(mini->exit_status));
	value = get_value_env(mini, var_name);
	if (value)
		return (ft_strdup(value));
	return (NULL);
}

static char	*expand_dollar(t_minishell *mini, char *line, int *i)
{
	char	var_name[256];
	int		k;
	char	*value;

	k = 0;
	(*i)++;
	if (line[*i] == '?')
		var_name[k++] = line[(*i)++];
	else
		while (line[*i] && (ft_isalnum(line[*i]) || line[*i] == '_'))
			var_name[k++] = line[(*i)++];
	var_name[k] = '\0';
	value = get_variable_value(mini, var_name);
	return (value);
}

static void	expand_line_loop(t_minishell *mini, char *line,
		char *result, int *j)
{
	int		i;
	char	*value;

	i = 0;
	*j = 0;
	while (line[i])
	{
		if (line[i] == '$' && line[i + 1]
			&& (ft_isalnum(line[i + 1]) || line[i + 1] == '_'
				|| line[i + 1] == '?'))
		{
			value = expand_dollar(mini, line, &i);
			if (value)
				append_to_result(result, value, j);
		}
		else
			result[(*j)++] = line[i++];
	}
}

char	*expand_heredoc_line(t_minishell *mini, char *line, int expand_vars)
{
	char	*result;
	int		j;

	if (!expand_vars || !line)
		return (ft_strdup(line));
	result = malloc(1024);
	if (!result)
		return (NULL);
	result[0] = '\0';
	expand_line_loop(mini, line, result, &j);
	result[j] = '\0';
	return (result);
}
