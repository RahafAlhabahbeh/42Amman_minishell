/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 22:15:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_value_env(t_minishell *mini, const char *key)
{
	t_env	*cur;

	cur = mini->env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*dir_slash;
	char	*full;

	if (!dir || !cmd)
		return (NULL);
	dir_slash = ft_strjoin(dir, "/");
	if (!dir_slash)
		return (NULL);
	full = ft_strjoin(dir_slash, cmd);
	free(dir_slash);
	return (full);
}

t_env	*find_env_node(t_env *env_list, const char *key)
{
	t_env	*cur;

	cur = env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur);
		cur = cur->next;
	}
	return (NULL);
}

char	*merge_args(char **argv, int start, int *consumed)
{
	char	*res;
	char	*tmp;
	int		i;

	*consumed = 1;
	if (!argv[start])
		return (ft_strdup(""));
	res = ft_strdup(argv[start]);
	if (!res)
		return (NULL);
	i = start + 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0' || ft_strchr(argv[i], '='))
		{
			tmp = ft_strjoin(res, argv[i]);
			free(res);
			res = tmp;
			(*consumed)++;
		}
		else
			break ;
		i++;
	}
	return (res);
}
