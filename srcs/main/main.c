/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/21 13:44:59 by dal-mahr         ###   ########.fr       */
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

    while (1)
    {
        init_shell(&minishell);

        if (!minishell.promp_input || minishell.promp_input[0] == '\0')
        {
            // free(minishell.promp_input);
            continue;
        }

        minishell.token = tokenize(&minishell);
        if (!minishell.token)
        {
            // free(minishell.promp_input);
            continue;
        }

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

        count_pipe(&minishell);
        printf("PIPE COUNT: %d", minishell.pipex_count);

        // // Allocate command array
        if (minishell.cmd)
            free_commands(&minishell);
        minishell.cmd = malloc(sizeof(t_cmd) * (minishell.pipex_count + 1));

        if (!minishell.cmd)
            exit(1);

        put_token_to_commands(&minishell);

        t_cmd *curr_cmd = minishell.cmd;
        for (int i = 0; i <= minishell.pipex_count; i++)
        {
            printf("\nCommand %d:\n", i);
            char **args = curr_cmd[i].argv;
            for (int j = 0; args && args[j]; j++)
            {
                printf("  argv[%d]: %s\n", j, args[j]);
            }
            if (curr_cmd[i].input_file_name)
                printf("  input: %s\n", curr_cmd[i].input_file_name);
            if (curr_cmd[i].output_file_name)
                printf("  output: %s\n", curr_cmd[i].output_file_name);
        }

        // check error IMP
        for (int i = 0; i <= minishell.pipex_count; i++)
        {
            printf("\nCommand %d:\n", i);
            if (!minishell.cmd[i].argv)
            {
                printf("  argv is NULL!\n");
                continue;
            }
            for (int j = 0; minishell.cmd[i].argv[j]; j++)
            {
                printf("  argv[%d]: %s\n", j, minishell.cmd[i].argv[j]);
            }
        }

        execute_command(&minishell, envp);

        free_tokens(minishell.token);
        minishell.token = NULL;

        free(minishell.promp_input);
        minishell.promp_input = NULL;
    }
    rl_clear_history();
    free_minishell(&minishell);
    printf("TEST LEAK\n");
    return 0;
}
