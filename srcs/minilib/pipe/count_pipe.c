#include "../../../include/minishell.h"

void count_pipe(t_minishell *minishell)
{
    int count = 0;
    t_token *cur = minishell->token;

    while (cur)
    {
        if (cur->type == PIPE)
            count++;
        cur = cur->next;
    }
    minishell->pipex_count = count;
}

