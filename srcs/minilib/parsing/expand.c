/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	extract_var_name(const char *str, int *pos,
		char *var_name, int *is_braced)
{
	int	i;
	int	k;

	i = *pos;
	k = 0;
	*is_braced = 0;
	if (str[i] == '{')
	{
		*is_braced = 1;
		i++;
		while (str[i] && str[i] != '}' && k < 255)
		{
			if (ft_isalnum(str[i]) || str[i] == '_' ||
				str[i] == '?' || str[i] == '$')
				var_name[k++] = str[i];
			else
				break ;
			i++;
		}
		if (str[i] == '}')
			i++;
		else
			return (-1);
	}
	else
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && k < 255)
			var_name[k++] = str[i++];
	}
	var_name[k] = '\0';
	*pos = i;
	return (k);
}

static char	*safe_resize_buffer(char *buffer, size_t *capacity, size_t needed)
{
	size_t	new_capacity;
	char	*new_buffer;

	if (needed < *capacity)
		return (buffer);
	new_capacity = *capacity;
	while (new_capacity <= needed)
		new_capacity *= 2;
	new_buffer = realloc(buffer, new_capacity);
	if (!new_buffer)
		return (NULL);
	*capacity = new_capacity;
	return (new_buffer);
}

static int	handle_special_var(t_minishell *mini, char **result,
		size_t *capacity, int *j, const char *str, int *i)
{
	char	*status_str;
	char	*pid_str;
	char	*temp;
	size_t	len;

	if (str[*i] == '?')
	{
		(*i)++;
		status_str = ft_itoa(mini->exit_status);
		if (!status_str)
			return (-1);
		len = ft_strlen(status_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
			return (free(status_str), -1);
		*result = temp;
		ft_strlcpy(*result + *j, status_str, len + 1);
		*j += len;
		free(status_str);
		return (1);
	}
	else if (str[*i] == '$')
	{
		(*i)++;
		pid_str = ft_itoa(getpid());
		if (!pid_str)
			return (-1);
		len = ft_strlen(pid_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
			return (free(pid_str), -1);
		*result = temp;
		ft_strlcpy(*result + *j, pid_str, len + 1);
		*j += len;
		free(pid_str);
		return (1);
	}
	return (0);
}

static int	handle_variable_expansion(t_minishell *mini, const char *str,
		char **result, size_t *capacity, int *i, int *j)
{
	char	var[256];
	int		is_braced;
	int		var_len;
	const char	*val;
	char	*temp;
	size_t	len;
	char	*status_str;
	char	*pid_str;

	ft_memset(var, 0, sizeof(var));
	var_len = extract_var_name(str, i, var, &is_braced);
	if (var_len < 0 || var_len == 0)
	{
		temp = safe_resize_buffer(*result, capacity, *j + 2);
		if (!temp)
			return (-1);
		*result = temp;
		(*result)[(*j)++] = '$';
		(*result)[*j] = '\0';
		return (0);
	}
	if (var_len == 1 && var[0] == '?')
	{
		status_str = ft_itoa(mini->exit_status);
		if (!status_str)
			return (-1);
		len = ft_strlen(status_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
		{
			free(status_str);
			return (-1);
		}
		*result = temp;
		ft_strlcat(*result + *j, status_str, len + 1);
		*j += len;
		free(status_str);
		return (0);
	}
	if (var_len == 1 && var[0] == '$')
	{
		pid_str = ft_itoa(getpid());
		if (!pid_str)
			return (-1);
		len = ft_strlen(pid_str);
		temp = safe_resize_buffer(*result, capacity, *j + len + 1);
		if (!temp)
		{
			free(pid_str);
			return (-1);
		}
		*result = temp;
		ft_strlcat(*result + *j, pid_str, len + 1);
		*j += len;
		free(pid_str);
		return (0);
	}
	val = get_value_env(mini, var);
	if (!val)
		val = "";
	len = ft_strlen(val);
	temp = safe_resize_buffer(*result, capacity, *j + len + 1);
	if (!temp)
		return (-1);
	*result = temp;
	ft_strlcat(*result + *j, val, len + 1);
	*j += len;
	return (0);
}

static int	process_string_expansion(t_minishell *mini, const char *str,
					char **result, size_t *capacity, int *i, int *j)
{
	char	*temp;
	int		special_result;

	while (str[*i])
	{
		if (str[*i] == '\x01' && str[*i + 1] == '$')
		{
			temp = safe_resize_buffer(*result, capacity, *j + 2);
			if (!temp)
				return (-1);
			*result = temp;
			(*result)[(*j)++] = '$';
			(*result)[*j] = '\0';
			*i += 2;
		}
		else if (str[*i] == '$' && str[*i + 1])
		{
			(*i)++;
			special_result = handle_special_var(mini, result, capacity, j, str, i);
			if (special_result == -1)
				return (-1);
			if (special_result == 0)
			{
				if (handle_variable_expansion(mini, str, result, capacity, i, j)
					== -1)
					return (-1);
			}
		}
		else
		{
			temp = safe_resize_buffer(*result, capacity, *j + 2);
			if (!temp)
				return (-1);
			*result = temp;
			(*result)[(*j)++] = str[(*i)++];
			(*result)[*j] = '\0';
		}
	}
	return (0);
}

static char	*expand_tilde(t_minishell *minishell, const char *str)
{
	const char	*home;
	char		*home_to_free;
	size_t		home_len;
	size_t		rest_len;
	char		*result;

	home_to_free = NULL;
	if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
	{
		home = get_value_env(minishell, "HOME");
		if (!home)
		{
			home_to_free = get_user_home_dir();
			home = home_to_free;
			if (!home)
				home = "";
		}
		home_len = ft_strlen(home);
		rest_len = ft_strlen(str + 1);
		result = (char *)malloc(home_len + rest_len + 1);
		if (!result)
		{
			if (home_to_free)
				free(home_to_free);
			return (NULL);
		}
		ft_memcpy(result, home, home_len);
		ft_memcpy(result + home_len, str + 1, rest_len);
		result[home_len + rest_len] = '\0';
		if (home_to_free)
			free(home_to_free);
		return (result);
	}
	return (ft_strdup(str));
}

char	*replace_var(t_minishell *minishell, const char *str, char quote)
{
	size_t	capacity;
	char	*result;
	int		i;
	int		j;
	char	*tilde_expanded;

	if (quote == '\'')
		return (ft_strdup(str));
	tilde_expanded = expand_tilde(minishell, str);
	if (!tilde_expanded)
		return (NULL);
	capacity = 1024;
	result = (char *)malloc(capacity);
	if (!result)
	{
		free(tilde_expanded);
		return (NULL);
	}
	result[0] = '\0';
	i = 0;
	j = 0;
	if (process_string_expansion(minishell, tilde_expanded,
		&result, &capacity, &i, &j) == -1)
	{
		free(result);
		free(tilde_expanded);
		return (NULL);
	}
	result[j] = '\0';
	free(tilde_expanded);
	return (result);
}

t_token	*expand(t_minishell *minishell)
{
	t_token	*cur;
	t_token	*new_list;
	t_token	*tail;

	cur = minishell->token;
	new_list = NULL;
	tail = NULL;
	while (cur)
	{
		char	*expanded;

		expanded = replace_var(minishell, cur->value, cur->quote);
		if (!expanded)
		{
			while (new_list)
			{
				t_token	*tmp;

				tmp = new_list;
				new_list = new_list->next;
				free(tmp->value);
				free(tmp);
			}
			return (NULL);
		}
		if (!cur->quote && ft_strchr(expanded, ' '))
		{
			char	**words;
			int		i;

			words = ft_split(expanded, ' ');
			free(expanded);
			if (!words)
			{
				while (new_list)
				{
					t_token *tmp = new_list;
					new_list = new_list->next;
					free(tmp->value);
					free(tmp);
				}
				return NULL;
			}
			i = 0;
			while (words[i])
			{
				t_token	*new_tok;

				new_tok = malloc(sizeof(t_token));
				if (!new_tok)
				{
					for (int j = i; words[j]; j++)
						free(words[j]);
					free(words);
					while (new_list)
					{
						t_token *tmp = new_list;
						new_list = new_list->next;
						free(tmp->value);
						free(tmp);
					}
					return NULL;
				}
				new_tok->value = ft_strdup(words[i]);
				new_tok->type = cur->type;
				new_tok->quote = 0;
				new_tok->next = NULL;
				if (!new_tok->value)
				{
					free(new_tok);
					for (int j = i; words[j]; j++)
						free(words[j]);
					free(words);
					while (new_list)
					{
						t_token *tmp = new_list;
						new_list = new_list->next;
						free(tmp->value);
						free(tmp);
					}
					return NULL;
				}
				if (!new_list)
					new_list = new_tok;
				else
					tail->next = new_tok;
				tail = new_tok;
				free(words[i]);
				i++;
			}
			free(words);
		}
		else
		{
			t_token	*new_tok;

			new_tok = malloc(sizeof(t_token));
			if (!new_tok)
			{
				free(expanded);
				while (new_list)
				{
					t_token *tmp = new_list;
					new_list = new_list->next;
					free(tmp->value);
					free(tmp);
				}
				return NULL;
			}
			new_tok->value = expanded;
			new_tok->type = cur->type;
			new_tok->quote = cur->quote;
			new_tok->next = NULL;
			if (!new_list)
				new_list = new_tok;
			else
				tail->next = new_tok;
			tail = new_tok;
		}
		cur = cur->next;
	}
	return (new_list);
}
