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
void free_commands(t_cmd *cmds, int count)
{
    for (int i = 0; i < count; i++)
    {
        // Free argv array strings
        for (int j = 0; cmds[i].argv && cmds[i].argv[j]; j++)
            free(cmds[i].argv[j]);
        free(cmds[i].argv);

        // Free input/output filenames if allocated
        if (cmds[i].input_file_name)
            free(cmds[i].input_file_name);
        if (cmds[i].output_file_name)
            free(cmds[i].output_file_name);
    }
    free(cmds);
}


// void free_commands(t_minishell *mini)
// {
// 	if (!mini->cmd)
// 		return;

// 	for (int i = 0; i <= mini->pipex_count; i++)
// 	{
// 		if (mini->cmd[i].argv)
// 		{
// 			for (int j = 0; mini->cmd[i].argv[j]; j++)
// 				free(mini->cmd[i].argv[j]);
// 			free(mini->cmd[i].argv);
// 		}
// 		free(mini->cmd[i].input_file_name);
// 		free(mini->cmd[i].output_file_name);
// 	}
// 	free(mini->cmd);
// 	mini->cmd = NULL;
// }




// void free_commands(t_minishell *minishell)
// {
//     for (int i = 0; i <= minishell->pipex_count; i++)
//     {
//         char **argv = minishell->cmd[i].argv;
//         if (argv)
//         {
//             for (int j = 0; argv[j]; j++)
//                 free(argv[j]);
//             free(argv);
//         }
//         if (minishell->cmd[i].input_file_name)
//             free(minishell->cmd[i].input_file_name);
//         if (minishell->cmd[i].output_file_name)
//             free(minishell->cmd[i].output_file_name);
//     }
//     free(minishell->cmd);
//     minishell->cmd = NULL;
// }

void free_minishell(t_minishell *mini)
{
    if (mini->promp_input)
        free(mini->promp_input);

    free_tokens(mini->token);

    // If you use an array of commands, free accordingly:
    free_cmds_array(mini->cmd, mini->pipex_count);

    // free_env_list(mini->env_list); // uncomment if you use env_list
}
