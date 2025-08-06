
#include "../../../include/minishell.h"

void init_shell(t_minishell *minishell)
{
    char *line;
    char *temp;
    char *full_input = NULL;
    int len;

    // Read the first line
    line = readline("minishell> ");

    // Handle EOF (Ctrl+D) - exit shell cleanly
    if (!line)
    {
        fprintf(stderr, "exit\n");
        rl_clear_history();
        free_minishell(minishell);
        exit(0);
    }

    // Handle empty input (just Enter pressed) - skip processing
    if (line[0] == '\0')
    {
        free(line);
        minishell->promp_input = NULL;
        return;
    }

    // Start building the full input
    full_input = strdup(line);
    if (!full_input)
    {
        free(line);
        minishell->promp_input = NULL;
        return;
    }

    // Check if line ends with pipe and continue reading
    len = strlen(line);
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
                fprintf(stderr, "exit\n");
                rl_clear_history();
                free(full_input);
                free_minishell(minishell);
                exit(0);
            }
            
            // Append the new line to full input
            temp = full_input;
            full_input = malloc(strlen(temp) + strlen(line) + 2); // +2 for space and null terminator
            if (!full_input)
            {
                free(temp);
                free(line);
                minishell->promp_input = NULL;
                return;
            }
            sprintf(full_input, "%s %s", temp, line);
            free(temp);
            
            len = strlen(line);
        }
        else
            break;
    }

    free(line);
    minishell->promp_input = full_input;

    // Add non-empty input to history
    add_history(minishell->promp_input);
}
