/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   merge_equal_sign_tokens.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/16 12:15:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	shift_argv_left(char **argv, int start)
{
	int	i;

	i = start;
	while (argv[i + 1])
	{
		argv[i] = argv[i + 1];
		i++;
	}
	argv[i] = NULL;
}

static void	merge_if_needed(t_cmd *cmd, int *index)
{
	char	*merged;

	if (ft_strlen(cmd->argv[*index]) > 0
		&& cmd->argv[*index][ft_strlen(cmd->argv[*index]) - 1] == '=')
	{
		merged = ft_strjoin(cmd->argv[*index], cmd->argv[*index + 1]);
		if (!merged)
		{
			write(2, "minishell: memory allocation failed\n", 36);
			exit(EXIT_FAILURE);
		}
		free(cmd->argv[*index]);
		free(cmd->argv[*index + 1]);
		cmd->argv[*index] = merged;
		shift_argv_left(cmd->argv, *index + 1);
		(*index)--;
	}
}

void	merge_equal_sign_tokens(t_cmd *cmd)
{
	int	j;

	j = 0;
	while (cmd->argv[j] && cmd->argv[j + 1])
	{
		merge_if_needed(cmd, &j);
		j++;
	}
}
