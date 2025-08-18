/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 21:43:30 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	parse_env_entry(t_minishell *mini, char *env_str)
{
	char	*eq;
	char	*key;
	char	*value;
	int		key_len;

	eq = ft_strchr(env_str, '=');
	if (!eq)
		return ;
	key_len = eq - env_str;
	key = ft_substr(env_str, 0, key_len);
	if (!key)
		return ;
	value = ft_strdup(eq + 1);
	if (!value)
	{
		free(key);
		return ;
	}
	mini->env_list = set_env_value(mini, key, value);
	free(key);
	free(value);
}
