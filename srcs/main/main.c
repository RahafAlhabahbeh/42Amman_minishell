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

int main(int ac, char **av, char **envp)
{
    t_minishell minishell;

    (void)ac;
    (void)av;
    init(&minishell);
    minishell.envp = envp;
    init_env_list(&minishell, envp);
    while (1)
    {
        setup_signals();
        init_shell(&minishell);

        if (!minishell.promp_input || minishell.promp_input[0] == '\0')
            continue;

        minishell.token = tokenize(&minishell);
        if (!minishell.token)
            continue;

        t_token *old = minishell.token;
        minishell.token = expand(&minishell);
        free_tokens(old);
        count_pipe(&minishell);
        if (minishell.cmd)
            free_commands(&minishell);
        minishell.cmd = malloc(sizeof(t_cmd) * (minishell.pipex_count + 1));

        if (!minishell.cmd)
            exit(1);

        put_token_to_commands(&minishell);
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
