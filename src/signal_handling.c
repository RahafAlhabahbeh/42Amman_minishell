/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 13:24:34 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/23 11:13:23 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Handler for SIGINT (Ctrl-C)
void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1); // Print newline
    rl_on_new_line();   // Move to a new line
    rl_replace_line("", 0); // Clear the current input line
    rl_redisplay();     // Redraw the prompt
}

// Setup signal handlers
void setup_signals(void)
{
    signal(SIGINT, handle_sigint); // Handle Ctrl-C
    signal(SIGQUIT, SIG_IGN);      // Ignore Ctrl-\"
    signal(SIGTSTP, SIG_IGN);      // Ignore Ctrl-Z
}

// To exit on Ctrl-D (EOF)
void handle_eof(void)
{
    write(1, "exit\n", 5);  // Optional: Print "exit" before quitting
    exit(0);  // Exit the shell
}

