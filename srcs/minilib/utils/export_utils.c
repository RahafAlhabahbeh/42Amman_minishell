/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_valid_identifier(const char *name)
{
	int	i;

	i = 0;
	if (!name || !name[0])
		return (0);
	if (!(ft_isalpha(name[0]) || name[0] == '_'))
		return (0);
	while (name[i] && name[i] != '=')
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

static int	cmp_env_key(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s1[i] != '=' && s2[i] && s2[i] != '=' && s1[i] == s2[i])
		i++;
	if ((s1[i] == '=' || s1[i] == '\0') && (s2[i] == '=' || s2[i] == '\0'))
		return (0);
	if (s1[i] == '=' || s1[i] == '\0')
		return (-1);
	if (s2[i] == '=' || s2[i] == '\0')
		return (1);
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

static void	manual_sort_env(char **arr, int count)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (i < count - 1)
	{
		j = 0;
		while (j < count - i - 1)
		{
			if (cmp_env_key(arr[j], arr[j + 1]) > 0)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;
			}
			j++;
		}
		i++;
	}
}

void	export_print_sorted_env(char **arr, int count)
{
	int		i;
	char	*eq;

	manual_sort_env(arr, count);
	i = 0;
	while (i < count)
	{
		eq = ft_strchr(arr[i], '=');
		if (!eq)
			printf("declare -x %s\n", arr[i]);
		else if (*(eq + 1) == '\0')
			printf("declare -x %.*s=\"\"\n", (int)(eq - arr[i]), arr[i]);
		else
			printf("declare -x %.*s=\"%s\"\n", (int)(eq - arr[i]), arr[i],
				eq + 1);
		free(arr[i]);
		i++;
	}
	free(arr);
}
