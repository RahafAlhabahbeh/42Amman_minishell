/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:58 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	unset_env(t_env **env, const char *key)
{
	t_env	*cur;
	t_env	*prev;

	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
		{
			if (prev)
				prev->next = cur->next;
			else
				*env = cur->next;
			free(cur->key);
			free(cur->value);
			free(cur);
			return ;
		}
		prev = cur;
		cur = cur->next;
	}
}

void	call_unset(t_minishell *mini, char **argv)
{
	int	has_error;
	int	i;

	has_error = 0;
	mini->exit_status = 0;
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
		{
			i++;
			continue ;
		}
		if (!is_valid_identifier(argv[i]))
		{
			has_error = 1;
			i++;
			continue ;
		}
		unset_env(&mini->env_list, argv[i]);
		i++;
	}
	if (has_error)
		mini->exit_status = 1;
}
