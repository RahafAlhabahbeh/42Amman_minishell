#include "../../../include/minishell.h"

void init_cmmands(t_minishell *minishell)
{
    int i;
    i = 0;
    while (i < minishell->pipex_count + 1)
    {
    minishell->cmd[i].argv = NULL;
    minishell->cmd[i].in_type = -1;
    minishell->cmd[i].input_file_name = NULL;
    minishell->cmd[i].out_type = -1;
    minishell->cmd[i].output_file_name = NULL;

    if (i < minishell->pipex_count)
        minishell->cmd[i].next = &minishell->cmd[i + 1];
    else
        minishell->cmd[i].next = NULL;
    i++;
    }
}