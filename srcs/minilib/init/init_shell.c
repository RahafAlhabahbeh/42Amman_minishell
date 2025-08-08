
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

    // Handle EOF (Ctrl+D) - exit shell cleanly
    if (!line)
    {
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

    // Check if line ends with pipe and continue reading
    len = ft_strlen(line);
    while (len > 0 && (line[len - 1] == '|' || isspace((unsigned char)line[len - 1])))
    {
        // Remove trailing spaces to check if it ends with pipe
        while (len > 0 && isspace((unsigned char)line[len - 1]))
            len--;

        if (len > 0 && line[len - 1] == '|')
        {
            // Line ends with pipe, read more input
            free(line);
            line = readline("> ");

            if (!line)
            {
                // EOF while waiting for more input
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
