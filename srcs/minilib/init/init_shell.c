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

static char	*handle_continuation_eof(t_minishell *ms, char *full_input)
{
	if (peek_sigint_received())
	{
		free(full_input);
		return (NULL);
	}
	write(2, "exit\n", 5);
	rl_clear_history();
	free(full_input);
	free_minishell(ms);
	exit(0);
}

static char	*append_continuation_line(char *full_input, char *line)
{
	char	*new_input;
	size_t	new_len;

	new_len = ft_strlen(full_input) + ft_strlen(line) + 2;
	new_input = malloc(new_len);
	if (!new_input)
	{
		free(full_input);
		free(line);
		return (NULL);
	}
	ft_strlcpy(new_input, full_input, new_len);
	ft_strlcat(new_input, " ", new_len);
	ft_strlcat(new_input, line, new_len);
	free(full_input);
	free(line);
	return (new_input);
}

static char	*handle_continuation(t_minishell *minishell, char *full_input)
{
	char	*line;

	line = readline("> ");
	if (peek_sigint_received())
	{
		if (line)
			free(line);
		free(full_input);
		return (NULL);
	}
	if (!line)
		return (handle_continuation_eof(minishell, full_input));
	return (append_continuation_line(full_input, line));
}

static char	*handle_readline_input(t_minishell *minishell)
{
	char	*line;

	line = readline("minishell> ");
	if (peek_sigint_received())
	{
		if (line)
			free(line);
		return (NULL);
	}
	if (!line)
	{
		if (peek_sigint_received())
			return (NULL);
		write(2, "exit\n", 5);
		rl_clear_history();
		free_minishell(minishell);
		exit(minishell->exit_status);
	}
	return (line);
}

static char	*process_pipe_continuation(t_minishell *ms, char *line, 
	char *full_input)
{
	int	len;

	len = ft_strlen(line);
	while (len > 0)
	{
		while (len > 0 && ft_isspace((unsigned char)line[len - 1]))
			len--;
		if (len > 0 && line[len - 1] == '|')
		{
			free(line);
			full_input = handle_continuation(ms, full_input);
			if (!full_input)
				return (NULL);
			line = ft_strdup(full_input);
			len = ft_strlen(line);
		}
		else
			break ;
	}
	free(line);
	return (full_input);
}

void	init_shell(t_minishell *minishell)
{
	char	*line;
	char	*full_input;
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	line = handle_readline_input(minishell);
	if (!line)
	{
		minishell->promp_input = NULL;
		return ;
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
	if (!has_command_content(line, ft_strlen(line)))
	{
		free(line);
		minishell->promp_input = full_input;
		add_history(minishell->promp_input);
		return ;
	}
	full_input = process_pipe_continuation(minishell, line, full_input);
	if (!full_input)
	{
		minishell->promp_input = NULL;
		return ;
	}
	minishell->promp_input = full_input;
	add_history(minishell->promp_input);
}
