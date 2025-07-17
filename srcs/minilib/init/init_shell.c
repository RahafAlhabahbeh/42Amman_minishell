
#include "../../../include/minishell.h"

// 1. Display the prompt.
// 2. Read the input line into a buffer. 
// 3. Add this line to the history.
// 4. Parsing the input.
// 5. Split it into tokens.
//? 6. Move it into cmd array.
// 7. After Each initialize check for Null.
// 8. exit if error occured

#include "../../../include/minishell.h"

void init_shell(t_minishell *minishell)
{
    minishell->promp_input = readline("minishell> ");

    // Handle EOF (Ctrl+D) - exit shell cleanly
    if (!minishell->promp_input)
    {
        printf("exit\n");
        exit(0);
    }

    // Handle empty input (just Enter pressed) - skip processing
    if (minishell->promp_input[0] == '\0')
    {
        free(minishell->promp_input);
        minishell->promp_input = NULL;
        return;
    }

    // Add non-empty input to history
    add_history(minishell->promp_input);
}

