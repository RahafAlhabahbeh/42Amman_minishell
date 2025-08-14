/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 17:30:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/14 12:31:01 by dal-mahr         ###   ########.fr       */
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

static int	cmp_env_key(const void *a, const void *b)
{
	char	*s1;
	char	*s2;
	int		i;

	s1 = *(char **)a;
	s2 = *(char **)b;
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

void	export_print_sorted_env(char **arr, int count)
{
	int		i;
	char	*eq;

	qsort(arr, count, sizeof(char *), cmp_env_key);
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
char *merge_args(char **argv, int start, int *consumed)
{
    char *res;
    char *tmp;
    int i;

    *consumed = 1;
    if (!argv[start])
        return ft_strdup("");

    res = ft_strdup(argv[start]);
    if (!res)
        return NULL;

    i = start + 1;
    while (argv[i])
    {
        // Merge empty tokens or tokens until we hit a new valid identifier
        if (argv[i][0] == '\0' || ft_strchr(argv[i], '='))
        {
            tmp = ft_strjoin(res, argv[i]);
            free(res);
            res = tmp;
            (*consumed)++;
        }
        else
            break;
        i++;
    }
    return res;
}

// char	*merge_args(char **argv, int start, int *consumed)
// {
// 	char	*res;
// 	char	*tmp;
// 	int		i;

// 	*consumed = 1;
// 	res = ft_strdup(argv[start]);
// 	i = start + 1;
// 	while (argv[i])
// 	{
// 		if (argv[i][0] == '\0')
// 		{
// 			tmp = ft_strjoin(res, argv[i]);
// 			free(res);
// 			res = tmp;
// 			(*consumed)++;
// 		}
// 		else
// 			break ;
// 		i++;
// 	}
// 	return (res);
// }
