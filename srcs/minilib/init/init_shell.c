
#include "../../../include/minishell.h"

void init_shell(t_minishell *minishell)
{
    minishell->promp_input = readline("minishell> ");

    // Handle EOF (Ctrl+D) - exit shell cleanly
    if (!minishell->promp_input)
    {
        fprintf(stderr, "exit\n");
        rl_clear_history();
        free_minishell(minishell);
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
