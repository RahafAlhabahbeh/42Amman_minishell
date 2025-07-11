/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/02 22:14:23 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//? Steps:
//* 1. Initialize some of elements in the structure.
//* 2. Each new prompt will initialize the rest of elements in the structure.
//* 3. Handle redirections.
//* 4. Forking.
//* 5. Free if allocated or exit.

// get path function
// update call_env function

int main(int ac, char **av, char **envp)
{
	(void) ac;
	(void) av;
	t_minishell minishell;
	int pid;

	init(&minishell);
	init_promp(&minishell, envp);	
	while (1)
	{
		init_shell(&minishell);
		redirection(&minishell);
		pid = fork();
		main_fork (&minishell, pid, envp);
		check_to_free (&minishell);
	}
	return (0);
}
