#include "../../../include/minishell.h"

void free_tokens(t_token *head)
{
    t_token *tmp;

    while (head)
    {
        tmp = head->next;
        if (head->value)
            free(head->value);
        free(head);
        head = tmp;
    }
}
void free_cmds_array(t_cmd *cmd_array, int count)
{
    if (!cmd_array) return;
    for (int i = 0; i <= count; i++)
    {
        if (cmd_array[i].argv)
        {
            for (int j = 0; cmd_array[i].argv[j]; j++)
                free(cmd_array[i].argv[j]);
            free(cmd_array[i].argv);
        }
        free(cmd_array[i].input_file_name);
        free(cmd_array[i].output_file_name);
    }
    free(cmd_array);
}
void free_commands(t_cmd *cmd, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (cmd[i].argv)
        {
            for (int j = 0; cmd[i].argv[j]; j++)
                free(cmd[i].argv[j]);
            free(cmd[i].argv);
        }
        free(cmd[i].input_file_name);
        free(cmd[i].output_file_name);
    }
    free(cmd);
}

void free_minishell(t_minishell *mini)
{
    if (mini->promp_input)
        free(mini->promp_input);

    free_tokens(mini->token);

    // If you use an array of commands, free accordingly:
    free_cmds_array(mini->cmd, mini->pipex_count);

    // free_env_list(mini->env_list); // uncomment if you use env_list
}
