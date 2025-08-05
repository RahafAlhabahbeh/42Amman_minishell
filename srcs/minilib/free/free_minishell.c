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
            cmd_array[i].argv = NULL;
        }
        if (cmd_array[i].input_file_name)
        {
            free(cmd_array[i].input_file_name);
            cmd_array[i].input_file_name = NULL;
        }
        if (cmd_array[i].output_file_name)
        {
            free(cmd_array[i].output_file_name);
            cmd_array[i].output_file_name = NULL;
        }
        if (cmd_array[i].heredoc_temp_file)
        {
            unlink(cmd_array[i].heredoc_temp_file);
            free(cmd_array[i].heredoc_temp_file);
            cmd_array[i].heredoc_temp_file = NULL;
        }
        if (cmd_array[i].heredoc_fd >= 0)
        {
            close(cmd_array[i].heredoc_fd);
            cmd_array[i].heredoc_fd = -1;
        }
    }
    free(cmd_array);
}
void free_commands(t_cmd *cmd, int count)
{
    if (!cmd) return;
    
    for (int i = 0; i <= count; i++)
    {
        if (cmd[i].argv)
        {
            for (int j = 0; cmd[i].argv[j]; j++)
                free(cmd[i].argv[j]);
            free(cmd[i].argv);
            cmd[i].argv = NULL;
        }
        if (cmd[i].input_file_name)
        {
            free(cmd[i].input_file_name);
            cmd[i].input_file_name = NULL;
        }
        if (cmd[i].output_file_name)
        {
            free(cmd[i].output_file_name);
            cmd[i].output_file_name = NULL;
        }
        if (cmd[i].heredoc_temp_file)
        {
            unlink(cmd[i].heredoc_temp_file);
            free(cmd[i].heredoc_temp_file);
            cmd[i].heredoc_temp_file = NULL;
        }
        if (cmd[i].heredoc_fd >= 0)
        {
            close(cmd[i].heredoc_fd);
            cmd[i].heredoc_fd = -1;
        }
    }
    free(cmd);
}

void free_minishell(t_minishell *mini)
{
    if (mini->promp_input)
        free(mini->promp_input);

    if (mini->token)
        free_tokens(mini->token);

    if (mini->cmd)
        free_cmds_array(mini->cmd, mini->pipex_count);

    if (mini->env_list)
        free_env_list(mini->env_list); // You must implement this
}

void free_env_list(t_env *env)
{
    t_env *tmp;
    while (env)
    {
        tmp = env->next;
        free(env->key);
        free(env->value);
        free(env);
        env = tmp;
    }
}

// void reset_minishell(t_minishell *mini)
// {
//     if (mini->promp_input)
//         free(mini->promp_input);
//     if (mini->token)
//         free_tokens(mini->token);
//     if (mini->cmd)
//         free_cmds_array(mini->cmd, mini->pipex_count);

//     mini->promp_input = NULL;
//     mini->token = NULL;
//     mini->cmd = NULL;
//     mini->cmd_count = 0;
//     mini->pipex_count = 0;
//     if (mini->promp_input)
//     {
//         free(mini->promp_input);
//         mini->promp_input = NULL;
//     }
//     if (mini->token)
//     {
//         free_tokens(mini->token);
//         mini->token = NULL;
//     }
//     if (mini->cmd)
//     {
//         free_commands(mini->cmd, mini->pipex_count);
//         mini->cmd = NULL;
//         mini->cmd_count = 0;
//     }
// }
void reset_minishell(t_minishell *mini)
{
    if (mini->promp_input)
    {
        free(mini->promp_input);
        mini->promp_input = NULL;
    }
    if (mini->token)
    {
        free_tokens(mini->token);
        mini->token = NULL;
    }
    if (mini->cmd)
    {
        free_commands(mini->cmd, mini->pipex_count);
        mini->cmd = NULL;
        mini->cmd_count = 0;
    }
    mini->pipex_count = 0;
}


