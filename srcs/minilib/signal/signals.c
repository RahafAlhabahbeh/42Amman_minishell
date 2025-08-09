#include "../../../include/minishell.h"

// Single global variable to store received signal number (constraint compliant)
// Note: Made non-static so heredoc.c can access it
volatile sig_atomic_t g_received_signal = 0;

// Global flag to track if child process is running (prevents double prompt)
static volatile sig_atomic_t g_child_running = 0;

// Global flag to track if we're in a child process (for pipe execution)
static volatile sig_atomic_t g_in_child_process = 0;

void handle_sigint(int sig)
{
    // Store only the signal number - no access to main data structures
    g_received_signal = sig;
    
    // Only display prompt and readline operations if no child is running
    if (!g_child_running)
    {
        write(1, "^C\n", 3);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_redisplay();
    }
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

// Function to check if SIGQUIT was received and reset the flag  
int check_sigquit_received(void)
{
    if (g_received_signal == SIGQUIT)
    {
        g_received_signal = 0;
        return 1;
    }
    return 0;
}

// Function to get the received signal without resetting
int get_received_signal(void)
{
    return g_received_signal;
}

// Function to reset the signal flag
void reset_received_signal(void)
{
    g_received_signal = 0;
}

// Functions to manage child process flag (prevents double prompt on SIGINT)
void set_child_running(int running)
{
    g_child_running = running ? 1 : 0;
}

int is_child_running(void)
{
    return g_child_running;
}

// Function to set minishell pointer (removed - violates constraint)
void set_minishell_pointer(t_minishell *mini)
{
    (void)mini; // No longer storing pointer to main data
}

void handle_sigquit(int sig)
{
    // Store signal for potential exit code handling
    g_received_signal = sig;
    // Ctrl+\ should do nothing in interactive mode (don't write anything)
}

void setup_signals(void)
{
    struct sigaction sa;
    
    // Setup SIGINT handler with sigaction for more reliable handling
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;  // Don't restart interrupted system calls - let readline be interrupted
    sigaction(SIGINT, &sa, NULL);
    
    // Setup SIGQUIT handler
    sa.sa_handler = handle_sigquit;
    sigaction(SIGQUIT, &sa, NULL);
    
    // Disable readline's signal handling
    extern int rl_catch_signals;
    rl_catch_signals = 0;
}

// Functions to track if we're in a child process (for pipe execution)
void set_in_child_process(int in_child)
{
    g_in_child_process = in_child;
}

int is_in_child_process(void)
{
    return g_in_child_process;
}