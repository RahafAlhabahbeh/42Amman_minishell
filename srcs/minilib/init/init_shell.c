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

static int	handle_empty_line(t_minishell *minishell, char *line)
{
	if (!line)
	{
		minishell->promp_input = NULL;
		return (1);
	}
	if (line[0] == '\0')
	{
		free(line);
		minishell->promp_input = NULL;
		return (1);
	}
	return (0);
}

static char	*duplicate_input(t_minishell *minishell, char *line)
{
	char	*full_input;

	full_input = ft_strdup(line);
	if (!full_input)
	{
		free(line);
		minishell->promp_input = NULL;
		return (NULL);
	}
	return (full_input);
}

static int	handle_no_command(t_minishell *minishell, char *line,
	char *full_input)
{
	if (!has_command_content(line, ft_strlen(line)))
	{
		free(line);
		minishell->promp_input = full_input;
		add_history(minishell->promp_input);
		return (1);
	}
	return (0);
}

void	init_shell(t_minishell *minishell)
{
	char		*line;
	char		*full_input;
	extern int	rl_catch_signals;

	rl_catch_signals = 0;
	line = handle_readline_input(minishell);
	if (handle_empty_line(minishell, line))
		return ;
	full_input = duplicate_input(minishell, line);
	if (!full_input)
		return ;
	if (handle_no_command(minishell, line, full_input))
		return ;
	full_input = process_pipe_continuation(minishell, line, full_input);
	if (!full_input)
	{
		minishell->promp_input = NULL;
		return ;
	}
	minishell->promp_input = full_input;
	add_history(minishell->promp_input);
}

/*
void	init_shell(t_minishell *minishell)
{
	char		*line;
	char		*full_input;
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
*/
