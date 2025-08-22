/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/22 17:31:20 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*create_env_string(t_env *env)
{
	char	*str;
	int		len;

	if (env->value)
	{
		len = ft_strlen(env->key) + ft_strlen(env->value) + 2;
		str = malloc(len);
		if (!str)
			return (NULL);
		ft_strlcpy(str, env->key, len);
		ft_strlcat(str, "=", len);
		ft_strlcat(str, env->value, len);
	}
	else
		str = ft_strdup(env->key);
	return (str);
}

static char	**create_env_array(t_minishell *mini)
{
	t_env	*cur;
	char	**arr;
	int		i;
	int		count;

	count = count_env_vars(mini->env_list);
	arr = malloc(sizeof(char *) * (count + 1));
	if (!arr)
		return (NULL);
	cur = mini->env_list;
	i = 0;
	while (cur)
	{
		arr[i] = create_env_string(cur);
		if (!arr[i])
		{
			free_env_array(arr, i);
			return (NULL);
		}
		cur = cur->next;
		i++;
	}
	arr[count] = NULL;
	export_print_sorted_env(arr, count);
	return (NULL);
}

static void	export_process_assignment(t_minishell *mini, char *merged)
{
	char		*eq;
	char		*key;
	char		*value;
	t_env		*existing;

	eq = ft_strchr(merged, '=');
	if (!eq)
	{
		existing = find_env_node(mini->env_list, merged);
		if (existing)
			return ;
		mini->env_list = set_env_value(mini, merged, NULL);
	}
	else
	{
		key = ft_substr(merged, 0, eq - merged);
		value = ft_strdup(eq + 1);
		mini->env_list = set_env_value(mini, key, value);
		free(key);
		free(value);
	}
}

static void	export_with_args(t_minishell *mini, char **argv)
{
	int	i;
	int	has_error;

	has_error = 0;
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0')
		{
			printf("export: `': not a valid identifier\n");
			has_error = 1;
		}
		else if (!is_valid_identifier(argv[i]))
		{
			printf("export: `%s': not a valid identifier\n", argv[i]);
			has_error = 1;
		}
		else
			export_process_assignment(mini, argv[i]);
		i++;
	}
	if (has_error)
		mini->exit_status = 1;
	else
		mini->exit_status = 0;
}

void	call_export(t_minishell *mini, char **argv)
{
	if (!argv[1])
	{
		create_env_array(mini);
		mini->exit_status = 0;
	}
	else
		export_with_args(mini, argv);
}
