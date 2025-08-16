/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredoc_line.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 19:00:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	reverse_str(char *str, int len)
{
	int		start;
	char	tmp;

	start = 0;
	while (start < len / 2)
	{
		tmp = str[start];
		str[start] = str[len - start - 1];
		str[len - start - 1] = tmp;
		start++;
	}
}

static char	*get_exit_status_str(int status)
{
	char	str[16];
	int		i;
	int		num;

	num = status;
	i = 0;
	if (status == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (ft_strdup(str));
	}
	while (num > 0)
	{
		str[i] = (num % 10) + '0';
		num /= 10;
		i++;
	}
	str[i] = '\0';
	reverse_str(str, i);
	return (ft_strdup(str));
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

char	*expand_heredoc_line(t_minishell *mini, char *line, int expand_vars)
{
	char	result[1024];
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
		if (line[i] == '$' && line[i + 1] != '\0' && 
			(ft_isalnum(line[i + 1]) || line[i + 1] == '_' 
			|| line[i + 1] == '?'))
		{
			char	var_name[256];
			int		k;

			i++;
			k = 0;
			if (line[i] == '?')
			{
				var_name[k++] = line[i];
				i++;
			}
			else
				while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
					var_name[k++] = line[i++];
			var_name[k] = '\0';
			append_var(result, &j, get_var_value(mini, var_name));
		}
		else
			result[j++] = line[i++];
	}
	result[j] = '\0';
	return (ft_strdup(result));
}
