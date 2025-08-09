
#include "../../../include/minishell.h"

// Simple strcpy implementation
static char *simple_strcpy(char *dest, const char *src)
{
    char *ptr = dest;
    while (*src)
        *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

// Simple strcat implementation  
static char *simple_strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr)
        ptr++;
    while (*src)
        *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

// Custom implementation to replace ctype.h isspace
static int ft_isspace(unsigned char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

void init_shell(t_minishell *minishell)
{
    char *line;
    char *temp;
    char *full_input = NULL;
    int len;

    // Ensure readline doesn't catch signals
    extern int rl_catch_signals;
    rl_catch_signals = 0;

    // Read the first line
    line = readline("minishell> ");

    // Check if SIGINT was received during readline (Ctrl+C)
    // Don't consume the signal here - let main loop handle it
    if (peek_sigint_received())
    {
        if (line)
            free(line);
        minishell->promp_input = NULL;
        return;
    }

    // Handle NULL return from readline
    if (!line)
    {
        // Check if this was due to a signal interruption
        if (peek_sigint_received())
        {
            // SIGINT interrupted readline - return to let main loop handle it
            minishell->promp_input = NULL;
            return;
        }
        
        // True EOF (Ctrl+D) - exit shell cleanly
        write(2, "exit\n", 5);
        rl_clear_history();
        free_minishell(minishell);
        exit(minishell->exit_status);
    }

    // Handle empty input (just Enter pressed) - skip processing
    if (line[0] == '\0')
    {
        free(line);
        minishell->promp_input = NULL;
        return;
    }

    // Start building the full input
    full_input = ft_strdup(line);
    if (!full_input)
    {
        free(line);
        minishell->promp_input = NULL;
        return;
    }

    // Check if input contains only pipes and whitespace - these should be syntax errors
    len = ft_strlen(line);
    int has_non_pipe_content = 0;
    for (int i = 0; i < len; i++)
    {
        if (!ft_isspace((unsigned char)line[i]) && line[i] != '|')
        {
            has_non_pipe_content = 1;
            break;
        }
    }
    
    // If input contains only pipes and spaces, don't show continuation prompt
    // This will allow the syntax checker to catch the error later
    if (!has_non_pipe_content)
    {
        // Input like "|", "|||", "| |", etc. - let tokenizer and syntax checker handle it
        free(line);
        minishell->promp_input = full_input;
        add_history(minishell->promp_input);
        return;
    }
    
    // Check if line ends with pipe and continue reading (only for legitimate commands)
    while (len > 0)
    {
        // Remove trailing spaces to check if it ends with pipe
        while (len > 0 && ft_isspace((unsigned char)line[len - 1]))
            len--;

        if (len > 0 && line[len - 1] == '|')
        {
            // Line ends with pipe and has actual command content, read more input
            free(line);
            line = readline("> ");

            // Check if SIGINT was received during continuation readline
            if (peek_sigint_received())
            {
                if (line)
                    free(line);
                free(full_input);
                minishell->promp_input = NULL;
                return;
            }

            if (!line)
            {
                // Check if this was due to signal interruption
                if (peek_sigint_received())
                {
                    // SIGINT interrupted continuation readline
                    free(full_input);
                    minishell->promp_input = NULL;
                    return;
                }
                
                // True EOF while waiting for more input
                write(2, "exit\n", 5);
                rl_clear_history();
                free(full_input);
                free_minishell(minishell);
                exit(0);
            }

            // Append the new line to full input
            temp = full_input;
            full_input = malloc(ft_strlen(temp) + ft_strlen(line) + 2); // +2 for space and null terminator
            if (!full_input)
            {
                free(temp);
                free(line);
                minishell->promp_input = NULL;
                return;
            }
            // Use simple_strcpy and simple_strcat instead of sprintf
            simple_strcpy(full_input, temp);
            simple_strcat(full_input, " ");
            simple_strcat(full_input, line);
            free(temp);

            len = ft_strlen(line);
        }
        else
            break;
    }

    free(line);
    minishell->promp_input = full_input;

    // Add non-empty input to history
    add_history(minishell->promp_input);
}
