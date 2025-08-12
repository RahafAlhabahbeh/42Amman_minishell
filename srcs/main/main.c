/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 06:28:39 by dal-mahr         ###   ########.fr       */
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

    // Setup signals once, not every iteration
    setup_signals();

    // minishell_loop function
    while (1)
    {
        init_shell(&minishell);
        
        // Check for SIGINT after init_shell and set proper exit status (130)
        if (check_sigint_received())
        {
            minishell.exit_status = 130;
            reset_minishell(&minishell);
            continue;
        }

        // Check for SIGQUIT after init_shell (should be ignored but handled)
        if (check_sigquit_received())
        {
            // SIGQUIT in interactive mode is ignored, no exit status change
            reset_minishell(&minishell);
            continue;
        }
        
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

        // Parse tokens into commands, checking for redirection errors
        if (put_token_to_commands(&minishell) == 0)
        {
            // print_commands(minishell.cmd);
            execute_command(&minishell, envp);
        }
        // If parsing failed, exit_status is already set by put_token_to_commands
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
