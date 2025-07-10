/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_to_free.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:25:59 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:28:29 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void check_to_free (t_minishell *minishell)
{
    	if (strcmp(minishell->input, "exit") == 0)
			exit(1);
		if (minishell->cmd)
		    free_2d(minishell->cmd);
		if (minishell->tok)
		    free_2d(minishell->tok);
		if (minishell->input)
		    free(minishell->input);
}