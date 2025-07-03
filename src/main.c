/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:39 by ral-haba          #+#    #+#             */
/*   Updated: 2025/07/03 14:12:54 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
    char    *input;
    t_token *tokens;
    t_token *cur;

    (void)argc;
    (void)argv;
    (void)envp;
    while (1)
    {
        input = readline("minishell$ ");
        if (!input)  // CTRL-D
        {
            printf("exit\n");
            break;
        }
        if (*input)
            add_history(input);

        tokens = lexer(input);
        cur = tokens;
        while (cur)
        {
            printf("  [type %d] %s\n", cur->type, cur->value);
            cur = cur->next;
        }
        free_tokens(tokens);
        free(input);
    }
    return (0);
}

