/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/14 23:46:36 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//? Steps:
//* 1. Initialize some of elements in the structure.
//* 2. Each new prompt will initialize the rest of elements in the structure.
//* 3. Handle redirections.
//* 4. Forking.
//* 5. Free if allocated or exit.

// get path function
// update call_env function

// Simple main to test the tokenizer
int main(int ac, char **av, char **envp)
{
    t_minishell minishell;

    (void)ac;
    (void)av;


    init(&minishell);
	minishell.envp = envp;
    minishell.env_list = init_env_list(envp);

    while (1)
    {
        init_shell(&minishell);
        
        minishell.token = tokenize(&minishell);
        if (!minishell.token)
            exit(1);

        // Save old token list and replace it with expanded one
        t_token *old = minishell.token;
        minishell.token = expand(&minishell, minishell.envp);
        free_tokens(old);

        t_token *cur = minishell.token;
        while (cur)
        {
            printf("Token type %u, value '%s'\n", cur->type, cur->value);
            cur = cur->next;
        }

        // count_pipe(minishell);
    
        // // Allocate command array
        // minishell.cmd = malloc(sizeof(t_cmd) * (minishell.pipex_count + 1));
        // if (!minishell.cmd)
        //     ft_exit(minishell, "ERROR\nNULL CMD", EXIT_FAILURE);
            
        // // Initialize each command structure
        // init_cmmands(minishell);

        free_tokens(minishell.token);
        minishell.token = NULL;

        free(minishell.promp_input);
        minishell.promp_input = NULL;
    }


    return 0;
}
