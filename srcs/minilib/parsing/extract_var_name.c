/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_var_name.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/16 11:40:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	handle_braced_var(const char *str, int *i, char *var_name)
{
	int	k;

	k = 0;
	(*i)++;
	while (str[*i] && str[*i] != '}' && k < 255)
	{
		if (ft_isalnum(str[*i]) || str[*i] == '_'
			|| str[*i] == '?' || str[*i] == '$')
			var_name[k++] = str[*i];
		else
			break ;
		(*i)++;
	}
	if (str[*i] == '}')
	{
		(*i)++;
		return (k);
	}
	return (-1);
}

static int	handle_unbraced_var(const char *str, int *i, char *var_name)
{
	int	k;

	k = 0;
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_') && k < 255)
		var_name[k++] = str[(*i)++];
	return (k);
}

int	extract_var_name(const char *str, int *pos,
	char *var_name, int *is_braced)
{
	int	i;
	int	len;

	i = *pos;
	*is_braced = 0;
	if (str[i] == '{')
	{
		*is_braced = 1;
		len = handle_braced_var(str, &i, var_name);
		if (len < 0)
			return (-1);
	}
	else
		len = handle_unbraced_var(str, &i, var_name);
	var_name[len] = '\0';
	*pos = i;
	return (len);
}

char	*safe_resize_buffer(char *buffer, size_t *capacity, size_t needed)
{
	size_t	new_capacity;
	char	*new_buffer;

	if (needed < *capacity)
		return (buffer);
	new_capacity = *capacity;
	while (new_capacity <= needed)
		new_capacity *= 2;
	new_buffer = realloc(buffer, new_capacity);
	if (!new_buffer)
		return (NULL);
	*capacity = new_capacity;
	return (new_buffer);
}
/*
int	extract_var_name(const char *str, int *pos,
		char *var_name, int *is_braced)
{
	int	i;
	int	k;

	i = *pos;
	k = 0;
	*is_braced = 0;
	if (str[i] == '{')
	{
		*is_braced = 1;
		i++;
		while (str[i] && str[i] != '}' && k < 255)
		{
			if (ft_isalnum(str[i]) || str[i] == '_' ||
				str[i] == '?' || str[i] == '$')
				var_name[k++] = str[i];
			else
				break ;
			i++;
		}
		if (str[i] == '}')
			i++;
		else
			return (-1);
	}
	else
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && k < 255)
			var_name[k++] = str[i++];
	}
	var_name[k] = '\0';
	*pos = i;
	return (k);
}

char	*safe_resize_buffer(char *buffer, size_t *capacity, size_t needed)
{
	size_t	new_capacity;
	char	*new_buffer;

	if (needed < *capacity)
		return (buffer);
	new_capacity = *capacity;
	while (new_capacity <= needed)
		new_capacity *= 2;
	new_buffer = realloc(buffer, new_capacity);
	if (!new_buffer)
		return (NULL);
	*capacity = new_capacity;
	return (new_buffer);
}
*/
