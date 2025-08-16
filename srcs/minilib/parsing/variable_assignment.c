/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   variable_assignment.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/16 13:00:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_variable_assignment(const char *str)
{
	char	*eq_pos;
	int		key_len;
	char	*key;
	int		is_valid;

	if (!str)
		return (0);
	eq_pos = ft_strchr(str, '=');
	if (!eq_pos)
		return (0);
	key_len = eq_pos - str;
	if (key_len == 0)
		return (0);
	key = ft_substr(str, 0, key_len);
	if (!key)
		return (0);
	is_valid = is_valid_identifier(key);
	free(key);
	return (is_valid);
}

void	handle_variable_assignment(t_minishell *mini, const char *assignment)
{
	char	*eq_pos;
	int		key_len;
	char	*key;
	char	*value;

	eq_pos = ft_strchr(assignment, '=');
	if (!eq_pos)
		return ;
	key_len = eq_pos - assignment;
	key = ft_substr(assignment, 0, key_len);
	value = ft_strdup(eq_pos + 1);
	if (key && value)
		mini->env_list = set_env_value(mini, key, value);
	free(key);
	free(value);
}

int	count_args_for_command(t_token *start)
{
	int	count;

	count = 0;
	while (start && start->type != PIPE)
	{
		if (start->type == WORD)
			count++;
		start = start->next;
	}
	return (count);
}
