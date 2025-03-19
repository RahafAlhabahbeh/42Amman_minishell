/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:52 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/19 14:12:18 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../include/minishell.h"

void shell_loop(char **envp)
{
    char *line;
    char **cmd;
    char *token;
    size_t bufsize = 1024;
	(void)envp;
	
    while (1) {
        line = readline("minishell> ");
        if (!line) {
            printf("\n");
            break;  // Exit on EOF (Ctrl-D)
        }
        add_history(line);
        cmd = malloc(bufsize * sizeof(char *));
        int i = 0;
        token = strtok(line, " ");
        while (token != NULL) {
            cmd[i] = token;
            token = strtok(NULL, " ");
            i++;
        }
        cmd[i] = NULL;
     /*    if (cmd[0] != NULL && strcmp(cmd[0], "exit") == 0) {
            execute_builtin(cmd, envp);
        }
        else {
            execute_command(cmd[0], cmd, envp);
        } */
        free(line);
        free(cmd);
    }
}

