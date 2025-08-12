/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*simple_strcat(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*ptr)
		ptr++;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

void	call_env(t_minishell *shell, char **argv)
{
	t_env	*curr;

	if (argv[1])
	{
		ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
		shell->exit_status = 1;
		return ;
	}
	curr = shell->env_list;
	while (curr)
	{
		if (curr->value)
		{
			ft_putstr_fd(curr->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(curr->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		curr = curr->next;
	}
}

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
	char	*full;

	if (!dir || !cmd)
		return (NULL);
	full = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2);
	if (!full)
		return (NULL);
	full[0] = '\0';
	simple_strcat(full, dir);
	simple_strcat(full, "/");
	simple_strcat(full, cmd);
	return (full);
}
