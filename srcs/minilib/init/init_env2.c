/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env2.c                                         :+:      :+:    :+:  */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	get_current_shlvl(t_minishell *mini)
{
	char	*shlvl_str;
	int		shlvl;

	shlvl_str = get_value_env(mini, "SHLVL");
	shlvl = 0;
	if (shlvl_str)
	{
		shlvl = ft_atoi(shlvl_str);
		if (shlvl < 0)
			shlvl = 0;
	}
	return (shlvl + 1);
}

static int	check_shlvl_limit(int shlvl)
{
	char	*shlvl_str_warning;

	if (shlvl > 999)
	{
		shlvl_str_warning = ft_itoa(shlvl);
		ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
		ft_putstr_fd(shlvl_str_warning, STDERR_FILENO);
		ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
		free(shlvl_str_warning);
		return (1);
	}
	return (shlvl);
}

static void	handle_shlvl(t_minishell *mini)
{
	char	*new_shlvl;
	int		shlvl;

	shlvl = get_current_shlvl(mini);
	shlvl = check_shlvl_limit(shlvl);
	new_shlvl = ft_itoa(shlvl);
	if (new_shlvl)
	{
		set_env_value(mini, "SHLVL", new_shlvl);
		free(new_shlvl);
	}
}

static void	init_pwd_env(t_minishell *mini)
{
	char	*pwd;
	char	*oldpwd;
	char	*current_pwd;
	char	cwd[1024];

	pwd = get_value_env(mini, "PWD");
	if (!pwd)
	{
		if (getcwd(cwd, sizeof(cwd)) != NULL)
			set_env_value(mini, "PWD", cwd);
	}
	oldpwd = get_value_env(mini, "OLDPWD");
	if (!oldpwd)
	{
		current_pwd = get_value_env(mini, "PWD");
		if (current_pwd)
			set_env_value(mini, "OLDPWD", current_pwd);
		else
			set_env_value(mini, "OLDPWD", "");
	}
}

void	init_env_list(t_minishell *mini, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		parse_env_entry(mini, envp[i]);
		i++;
	}
	handle_shlvl(mini);
	init_pwd_env(mini);
}
