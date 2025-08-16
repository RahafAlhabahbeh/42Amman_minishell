/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*get_env_value(const char *key, t_env *env)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

static t_env	*update_existing_env(t_env *env, char *key, char *value)
{
	t_env	*cur;

	cur = env;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			free(cur->value);
			if (value)
				cur->value = ft_strdup(value);
			else
				cur->value = NULL;
			return (env);
		}
		cur = cur->next;
	}
	return (NULL);
}

static t_env	*create_new_env_node(char *key, char *value)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->key = ft_strdup(key);
	if (!new_node->key)
	{
		free(new_node);
		return (NULL);
	}
	if (value)
	{
		new_node->value = ft_strdup(value);
		if (!new_node->value)
		{
			free(new_node->key);
			free(new_node);
			return (NULL);
		}
	}
	else
		new_node->value = NULL;
	new_node->next = NULL;
	return (new_node);
}

static void	add_env_to_list(t_env **env_list, t_env *new_node)
{
	t_env	*last;

	if (!*env_list)
		*env_list = new_node;
	else
	{
		last = *env_list;
		while (last->next)
			last = last->next;
		last->next = new_node;
	}
}

t_env	*set_env_value(t_minishell *mini, char *key, char *value)
{
	t_env	*updated;
	t_env	*new_node;

	updated = update_existing_env(mini->env_list, key, value);
	if (updated)
		return (updated);
	new_node = create_new_env_node(key, value);
	if (!new_node)
		return (mini->env_list);
	add_env_to_list(&mini->env_list, new_node);
	return (mini->env_list);
}
