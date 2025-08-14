/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/14 12:33:26 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int count_env_vars(t_env *env_list)
{
	int count;
	t_env *cur;

	count = 0;
	cur = env_list;
	while (cur)
	{
		count++;
		cur = cur->next;
	}
	return (count);
}

static char *create_env_string(t_env *env)
{
	char *str;
	int len;

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

static void free_env_array(char **arr, int count)
{
	while (count > 0)
		free(arr[--count]);
	free(arr);
}

static char **create_env_array(t_minishell *mini)
{
	t_env *cur;
	char **arr;
	int count;
	int i;

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

static void export_no_args(t_minishell *mini)
{
	printf("export: export_no_args\n");
	create_env_array(mini);
}

static void export_process_assignment(t_minishell *mini, char *merged)
{
	char *eq;
	char *key;
	char *value;

	eq = ft_strchr(merged, '=');
	if (!eq)
		mini->env_list = set_env_value(mini, merged, NULL);
	else
	{
		key = ft_substr(merged, 0, eq - merged);
		value = ft_strdup(eq + 1);
		mini->env_list = set_env_value(mini, key, value);
		free(key);
		free(value);
	}
}

static void export_with_args(t_minishell *mini, char **argv)
{
	int has_error;
	int i;
	int consumed;
	char *merged;

	has_error = 0;
	i = 1;
	while (argv[i])
	{
		if (argv[i][0] == '\0') // skip empty tokens
		{
			i++;
			continue;
		}
		merged = merge_args(argv, i, &consumed);
		if (merged[0] == '\0' || !is_valid_identifier(merged))
		{
			printf("export: `%s`: not a valid identifier\n", merged);
			has_error = 1;
			free(merged);
			i += consumed;
			continue;
		}
		if (!is_valid_identifier(merged))
		{
			printf("export: `%s`: not a valid identifier\n", merged);
			has_error = 1;
			free(merged);
			i += consumed;
			continue;
		}
		export_process_assignment(mini, merged);
		free(merged);
		i += consumed;
	}
	if (has_error)
		mini->exit_status = 1;
}

void call_export(t_minishell *mini, char **argv)
{
	if (!argv[1])
		export_no_args(mini);
	else
		export_with_args(mini, argv);
}
