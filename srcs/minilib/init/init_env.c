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

static void	parse_env_entry(t_minishell *mini, char *env_str)
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

void	init_env_list(t_minishell *mini, char **envp)
{
	int	i;

	mini->env_list = NULL;
	i = 0;
	while (envp[i])
	{
		parse_env_entry(mini, envp[i]);
		i++;
	}
	handle_shlvl(mini);
	init_pwd_env(mini);
}

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
