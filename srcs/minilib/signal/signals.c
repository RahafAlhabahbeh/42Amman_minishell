#include "../../../include/minishell.h"

// Global variable to track if we're in a child process
static int g_in_child_process = 0;
// Global variable to track if SIGINT was received
static int g_sigint_received = 0;
// Global pointer to minishell structure for setting exit status
static t_minishell *g_minishell = NULL;

void handle_sigint(int sig)
{
    (void)sig;
    write(1, "\n", 1);
    rl_replace_line("", 0);
    rl_on_new_line();
    // Only redisplay if we're not in a child process
    if (!g_in_child_process)
        rl_redisplay();
    
    // Mark that SIGINT was received
    g_sigint_received = 1;
    
    // Set exit status to 130 (128 + SIGINT signal number 2)
    if (g_minishell)
        g_minishell->exit_status = 130;
}

// Function to set child process flag
void set_child_process_flag(int flag)
{
    g_in_child_process = flag;
}

// Function to set minishell pointer for signal handling
void set_minishell_pointer(t_minishell *mini)
{
    g_minishell = mini;
}

// Function to check if SIGINT was received and reset the flag
int check_sigint_received(void)
{
    if (g_sigint_received)
    {
        g_sigint_received = 0;
        return 1;
    }
    return 0;
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