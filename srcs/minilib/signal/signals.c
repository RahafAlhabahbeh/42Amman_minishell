#include "../../../include/minishell.h"

// Single global variable to store received signal number (constraint compliant)
static volatile sig_atomic_t g_received_signal = 0;

void handle_sigint(int sig)
{
    // Store only the signal number - no access to main data structures
    g_received_signal = sig;
    write(1, "^C\n", 3);
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

// Function to check if SIGINT was received and reset the flag
int check_sigint_received(void)
{
    if (g_received_signal == SIGINT)
    {
        g_received_signal = 0;
        return 1;
    }
    return 0;
}

// Function to check if SIGINT was received WITHOUT resetting the flag
int peek_sigint_received(void)
{
    return (g_received_signal == SIGINT);
}

// Function to set child process flag (removed - no longer needed)
void set_child_process_flag(int flag)
{
    (void)flag; // No longer using global for this
}

// Function to set minishell pointer (removed - violates constraint)
void set_minishell_pointer(t_minishell *mini)
{
    (void)mini; // No longer storing pointer to main data
}

void handle_sigquit(int sig)
{
    (void)sig;
    // Ctrl+\ should do nothing in interactive mode
}

void setup_signals(void)
{
    struct sigaction sa;
    
    // Setup SIGINT handler with sigaction for more reliable handling
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;  // Restart interrupted system calls
    sigaction(SIGINT, &sa, NULL);
    
    // Setup SIGQUIT handler
    sa.sa_handler = handle_sigquit;
    sigaction(SIGQUIT, &sa, NULL);
    
    // Disable readline's signal handling
    extern int rl_catch_signals;
    rl_catch_signals = 0;
}