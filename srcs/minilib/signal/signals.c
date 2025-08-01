#include "../../../include/minishell.h"

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Ctrl+\ should do nothing in interactive mode
}

void setup_signals(void)
{
    signal(SIGINT, handle_sigint);   // Handle Ctrl+C
    signal(SIGQUIT, handle_sigquit); // Handle Ctrl+backslash
}