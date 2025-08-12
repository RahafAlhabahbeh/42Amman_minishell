/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static char	*simple_strcpy(char *dest, const char *src)
{
	char	*ptr;

	ptr = dest;
	while (*src)
		*ptr++ = *src++;
	*ptr = '\0';
	return (dest);
}

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

static int	ft_isspace(unsigned char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v'
		|| c == '\f' || c == '\r');
}

static int	has_command_content(char *line, int len)
{
	int	i;

	i = 0;
	while (i < len)
	{
		if (!ft_isspace((unsigned char)line[i]) && line[i] != '|')
			return (1);
		i++;
	}
	return (0);
}

static char	*handle_continuation(t_minishell *minishell, char *full_input)
{
	char	*line;
	char	*temp;
	char	*new_input;

	line = readline("> ");
	if (peek_sigint_received())
	{
		if (line)
			free(line);
		free(full_input);
		return (NULL);
	}
	if (!line)
	{
		if (peek_sigint_received())
		{
			free(full_input);
			return (NULL);
		}
		write(2, "exit\n", 5);
		rl_clear_history();
		free(full_input);
		free_minishell(minishell);
		exit(0);
	}
	temp = full_input;
	new_input = malloc(ft_strlen(temp) + ft_strlen(line) + 2);
	if (!new_input)
	{
		free(temp);
		free(line);
		return (NULL);
	}
	simple_strcpy(new_input, temp);
	simple_strcat(new_input, " ");
	simple_strcat(new_input, line);
	free(temp);
	free(line);
	return (new_input);
}

void	init_shell(t_minishell *minishell)
{
	char			*line;
	char			*full_input;
	int				len;
	extern int		rl_catch_signals;

	rl_catch_signals = 0;
	line = readline("minishell> ");
	if (peek_sigint_received())
	{
		if (line)
			free(line);
		minishell->promp_input = NULL;
		return ;
	}
	if (!line)
	{
		if (peek_sigint_received())
		{
			minishell->promp_input = NULL;
			return ;
		}
		write(2, "exit\n", 5);
		rl_clear_history();
		free_minishell(minishell);
		exit(minishell->exit_status);
	}
	if (line[0] == '\0')
	{
		free(line);
		minishell->promp_input = NULL;
		return ;
	}
	full_input = ft_strdup(line);
	if (!full_input)
	{
		free(line);
		minishell->promp_input = NULL;
		return ;
	}
	len = ft_strlen(line);
	if (!has_command_content(line, len))
	{
		free(line);
		minishell->promp_input = full_input;
		add_history(minishell->promp_input);
		return ;
	}
	while (len > 0)
	{
		while (len > 0 && ft_isspace((unsigned char)line[len - 1]))
			len--;
		if (len > 0 && line[len - 1] == '|')
		{
			free(line);
			full_input = handle_continuation(minishell, full_input);
			if (!full_input)
			{
				minishell->promp_input = NULL;
				return ;
			}
			line = ft_strdup(full_input);
			len = ft_strlen(line);
		}
		else
			break ;
	}
	free(line);
	minishell->promp_input = full_input;
	add_history(minishell->promp_input);
}