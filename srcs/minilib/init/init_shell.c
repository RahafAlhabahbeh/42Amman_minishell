
#include "../../../include/minishell.h"

// 1. Display the prompt.
// 2. Read the input line into a buffer. 
// 3. Add this line to the history.
// 4. Parsing the input.
// 5. Split it into tokens.
//? 6. Move it into cmd array.
// 7. After Each initialize check for Null.
// 8. exit if error occured

void init_shell(t_minishell *minishell)
{
	//minishell->input = readline(PROMPT);
	// char *promp = NULL;
	// promp = minishell_promp(minishell);
	minishell->promp_input = readline("minishell> ");
    // free(promp);
    if (!minishell->promp_input)
    {
        printf("EXIT");
        //ft_exit(); // free everything
        exit(EXIT_SUCCESS);
    }

	if (*minishell->promp_input)
		add_history(minishell->promp_input);
    
    if (!*minishell->promp_input)
    {
        free(minishell->promp_input);
        minishell->promp_input = NULL;
        return;
    }
}