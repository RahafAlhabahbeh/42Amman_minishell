/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_shell.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:35 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/03 00:48:50 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// 1. Display the prompt.
// 2. Read the input line into a buffer. 
// 3. Add this line to the history.
// 4. Parsing the input.
// 5. Split it into tokens.
//? 6. Move it into cmd array.
// 7. After Each initialize check for Null.
// 8. exit if error occured

void init_shell(t_minishell *minishell)
{
	minishell->input = readline(PROMPT);
	if (!minishell->input)
		ft_exit(minishell, "ERROR\nNULL input", EXIT_FAILURE);
	if (*minishell->input)
		add_history(minishell->input);
	minishell->tok = get_tokens (minishell->input);
	if (!minishell->tok)
    		ft_exit(minishell, "ERROR\nNULL tok", EXIT_FAILURE);
	minishell->count = 0;
	while (minishell->tok[minishell->count])
		minishell->count++;
	minishell->cmd = malloc(sizeof(char *) * (minishell->count + 1));
	if (!minishell->cmd)
		ft_exit(minishell, "ERROR\nNULL CMD", EXIT_FAILURE);
}
