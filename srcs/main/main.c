/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 13:10:46 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handle_signals_and_input(t_minishell *minishell)
{
	if (check_sigint_received())
	{
		if (minishell->exit_status == 0)
			minishell->exit_status = 130;
		reset_minishell(minishell);
		return (1);
	}
	if (check_sigquit_received())
	{
		if (minishell->exit_status == 0)
			minishell->exit_status = 131;
		reset_minishell(minishell);
		return (1);
	}
	if (!minishell->promp_input || minishell->promp_input[0] == '\0')
	{
		free(minishell->promp_input);
		minishell->promp_input = NULL;
		return (1);
	}
	return (0);
}

static int	check_malformed_redirections(char *input)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(input);
	while (i < len - 2)
	{
		// Look for pattern: > [space] digit > digit
		// This catches "ls > 1>2>3" where the first redirection is properly spaced
		// but subsequent ones are malformed
		if (input[i] == '>' && input[i + 1] == ' ')
		{
			int j = i + 2;
			// Skip any additional spaces
			while (j < len && input[j] == ' ')
				j++;
			// Check if we have digit>digit pattern
			if (j < len && ft_isdigit(input[j]))
			{
				int k = j;
				while (k < len && ft_isdigit(input[k]))
					k++;
				if (k < len && input[k] == '>')
				{
					ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
					write(2, &input[j], 1);  // Report the first digit in malformed sequence
					ft_putstr_fd("'\n", 2);
					return (0);
				}
			}
		}
		i++;
	}
	return (1);
}

static int	process_tokens_and_commands(t_minishell *minishell, char **envp)
{
	t_token	*old;

	if (!check_malformed_redirections(minishell->promp_input))
	{
		minishell->exit_status = 2;
		return (1);
	}
	minishell->token = tokenize(minishell);
	if (!minishell->token)
		return (1);
	if (!is_valid_syntax(minishell->token))
	{
		minishell->exit_status = 2;
		return (1);
	}
	old = minishell->token;
	minishell->token = expand(minishell);
	free_tokens(old);
	if (!minishell->token)
		return (1);
	count_pipe(minishell);
	init_cmd(minishell);
	if (put_token_to_commands(minishell) == 0)
		execute_command(minishell, envp);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_minishell	minishell;

	(void)ac;
	(void)av;
	init(&minishell);
	minishell.envp = envp;
	init_env_list(&minishell, envp);
	setup_signals();
	while (1)
	{
		init_shell(&minishell);
		if (handle_signals_and_input(&minishell))
			continue ;
		if (process_tokens_and_commands(&minishell, envp))
		{
			reset_minishell(&minishell);
			continue ;
		}
		reset_minishell(&minishell);
	}
	rl_clear_history();
	free_minishell(&minishell);
	return (0);
}
