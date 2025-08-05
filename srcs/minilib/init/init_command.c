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
    minishell->cmd[i].original_stdin = -1;
    minishell->cmd[i].original_stdout = -1;

    if (i < minishell->pipex_count)
        minishell->cmd[i].next = &minishell->cmd[i + 1];
    else
        minishell->cmd[i].next = NULL;
    i++;
    }
}

void init_cmd(t_minishell *mini)
{
    mini->cmd = malloc(sizeof(t_cmd) * (mini->pipex_count + 1));
    if (!mini->cmd)
        exit(EXIT_FAILURE);

    for (int i = 0; i <= mini->pipex_count; i++)
    {
        mini->cmd[i].argv = NULL;
        mini->cmd[i].input_file_name = NULL;
        mini->cmd[i].output_file_name = NULL;
        mini->cmd[i].in_type = -1;
        mini->cmd[i].out_type = -1;
        mini->cmd[i].input_quote = 0;
        mini->cmd[i].heredoc_temp_file = NULL;
        mini->cmd[i].heredoc_fd = -1;
        mini->cmd[i].next = (i < mini->pipex_count) ? &mini->cmd[i + 1] : NULL;
        mini->cmd[i].prev = (i > 0) ? &mini->cmd[i - 1] : NULL;
    }
}
