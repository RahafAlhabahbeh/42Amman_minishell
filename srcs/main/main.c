/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/05 05:24:14 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int main(int ac, char **av, char **envp)
{
    (void)ac;
    (void)av;
    t_minishell minishell;

    init(&minishell);
    minishell.envp = envp;
    init_env_list(&minishell, envp); // put this inside init

    // minishell_loop function
    while (1)
    {
        setup_signals();

        init_shell(&minishell);
        
        // Check if SIGINT was received during input
        if (check_sigint_received())
        {
            minishell.exit_status = 130;  // 128 + SIGINT (2)
            free(minishell.promp_input);
            minishell.promp_input = NULL;
            continue;
        }
        
        // printf("DEBUG: raw input = [%s]\n", minishell.promp_input);
        // if (!minishell.promp_input || minishell.promp_input[0] == '\0')
        //     continue;
        if (!minishell.promp_input || minishell.promp_input[0] == '\0')
        {
            free(minishell.promp_input);
            minishell.promp_input = NULL;
            continue;
        }
        // printf("DEBUG: raw input = [%s]\n", minishell.promp_input);

        minishell.token = tokenize(&minishell);
        if (!minishell.token)
            continue;

        if (!is_valid_syntax(minishell.token))
        {
            minishell.exit_status = 2;  // Incorrect usage
            reset_minishell(&minishell);
            continue;
        }
        t_token *old = minishell.token;
        minishell.token = expand(&minishell);
        free_tokens(old);
    
        if (!minishell.token)
        {
            // If expand failed, continue to next iteration
            continue;
        }

        // print_tokens(minishell.token);

        count_pipe(&minishell);
        // if (minishell.cmd)
        //     free_commands(minishell.cmd, minishell.pipex_count);

        // minishell.cmd_count = minishell.pipex_count + 1;
        // minishell.cmd = malloc(sizeof(t_cmd) * minishell.cmd_count);

        // if (!minishell.cmd)
        //     exit(EXIT_FAILURE);

        // ft_bzero(minishell.cmd, sizeof(t_cmd) * (minishell.pipex_count + 1));

        init_cmd(&minishell);

        put_token_to_commands(&minishell);
        // print_commands(minishell.cmd);
        execute_command(&minishell, envp);
        reset_minishell(&minishell);
        // free_tokens(minishell.token);
        // minishell.token = NULL;

        // free_tokens(minishell.token);
        // minishell.token = NULL;

        // free(minishell.promp_input);
        // minishell.promp_input = NULL;

    }

    rl_clear_history();
    free_minishell(&minishell);
    return 0;
}
