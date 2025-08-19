/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert_env_to_array.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/19 04:55:51 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*ft_strjoin_three(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*res;

	if (!s1 || !s2 || !s3)
		return (NULL);
	tmp = ft_strjoin(s1, s2);
	if (!tmp)
		return (NULL);
	res = ft_strjoin(tmp, s3);
	free(tmp);
	return (res);
}

static int	env_list_size(t_env *env)
{
	int	count;

	count = 0;
	while (env)
	{
		count++;
		env = env->next;
	}
	return (count);
}

char **convert_env_to_array(t_env *env)
{
    int     count;
    t_env   *tmp;
    char    **array;
    int     i;

    count = env_list_size(env);
    array = malloc(sizeof(char *) * (count + 1));
    if (!array)
        return (NULL);
    tmp = env;
    i = 0;
    while (tmp)
    {
        array[i] = ft_strjoin_three(tmp->key, "=", tmp->value);
        i++;
        tmp = tmp->next;
    }
    array[i] = NULL;
    return (array);
}

void free_env_array_2(char **arr)
{
    int i = 0;
    if (!arr) return;
    while (arr[i])
        free(arr[i++]);
    free(arr);
}
