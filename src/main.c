/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:39 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/19 14:08:50 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int main(int argc, char **argv, char **envp)
{
	if (argc < 3)
    {
        write(2, "Usage: my_shell <command> <file>\n", 33);
        return 1;
    }
    printf("Welcome to MiniShell! Type 'exit' to quit.\n");
	if (strcmp(argv[1], ">") == 0)
        redirect_output(argv[2], 0);  // Overwrite output

    if (strcmp(argv[1], ">>") == 0)
        redirect_output(argv[2], 1);  // Append output

    if (strcmp(argv[1], "<") == 0)
        redirect_input(argv[2]);  // Input redirection

    if (strcmp(argv[1], "<<") == 0)
        heredoc(argv[2]);  // Heredoc redirection
    shell_loop(envp);  // Start the shell loop
    return 0;
}
