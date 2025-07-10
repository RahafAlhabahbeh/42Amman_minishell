/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:42 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/02 22:14:43 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// Reinitialize i, j each new prompt.
// Loop over tokens array to search redirections.
// Set a null termiate in commands array

void redirection(t_minishell *shell)
{
	shell->i = 0;
	shell->j = 0;
	while (shell->tok[shell->i])
	{
		redir_compare1(shell);
	}
	shell->cmd[shell->j] = NULL;
}
