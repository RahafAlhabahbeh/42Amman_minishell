/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   continuation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 12:15:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/18 12:10:06 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	has_command_content(char *line, int len)
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

char	*handle_continuation_eof(t_minishell *ms, char *full_input)
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

char	*append_continuation_line(char *full_input, char *line)
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

int	check_received_signal(t_minishell *minishell, char *line)
{
	if (g_received_signal == 130)
	{
		minishell->exit_status = 130;
		g_received_signal = 0;
	}
	if (g_received_signal == 131)
	{
		minishell->exit_status = 131;
		g_received_signal = 0;
	}
	if (peek_sigint_received())
	{
		if (line)
			free(line);
		return (1);
	}
	return (0);
}

char	*handle_readline_input(t_minishell *minishell)
{
	char	*line;

	if (check_sigint_received())
	{
		minishell->exit_status = 130;
		return (NULL);
	}
	line = readline("minishell> ");
	if (check_received_signal(minishell, line))
		return (NULL);
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
