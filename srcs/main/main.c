/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:22 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/27 13:07:48 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_tokens(t_token *token)
{
    printf("\n🧩 Token List:\n");
    while (token)
    {
        const char *type_str;
        switch (token->type)
        {
            case WORD: type_str = "WORD"; break;
            case PIPE: type_str = "PIPE"; break;
            case REDIR_IN: type_str = "REDIR_IN"; break;
            case REDIR_OUT: type_str = "REDIR_OUT"; break;
            case REDIR_APPEND: type_str = "REDIR_APPEND"; break;
            case HERE_DOC: type_str = "HERE_DOC"; break;
            default: type_str = "UNKNOWN"; break;
        }

        printf("  [%s] \"%s\" (quote: %c)\n", type_str, token->value, token->quote ? token->quote : '-');
        token = token->next;
    }
    printf("🔚 End of Tokens\n");
}

void print_commands(t_cmd *cmd)
{
    int i = 0;
    printf("\n🚀 Command List:\n");
    while (cmd)
    {
        printf("  Cmd[%d]:\n", i);
        if (cmd->argv)
        {
            for (int j = 0; cmd->argv[j]; j++)
                printf("    argv[%d]: [%s]\n", j, cmd->argv[j]);
        }
        else
        {
            printf("    (no argv)\n");
        }

        switch (cmd->in_type)
        {
            case REDIR_IN: printf("    input: REDIR_IN <%s>\n", cmd->input_file_name); break;
            case HERE_DOC: printf("    input: HERE_DOC\n"); break;
            case PIPE:     printf("    input: PIPE\n"); break;
            default:       printf("    input: none\n"); break;
        }

        switch (cmd->out_type)
        {
            case REDIR_OUT:    printf("    output: REDIR_OUT >%s\n", cmd->output_file_name); break;
            case REDIR_APPEND: printf("    output: APPEND >>%s\n", cmd->output_file_name); break;
            case PIPE:         printf("    output: PIPE\n"); break;
            default:           printf("    output: none\n"); break;
        }

        cmd = cmd->next;
        i++;
    }
    printf("🔚 End of Commands\n");
}

int main(int ac, char **av, char **envp)
{
    t_minishell minishell;

    (void)ac;
    (void)av;

    init(&minishell);
    minishell.envp = envp;
    init_env_list(&minishell, envp);

    while (1)
    {
        setup_signals();
        init_shell(&minishell);

        if (!minishell.promp_input || minishell.promp_input[0] == '\0')
            continue;

        minishell.token = tokenize(&minishell);
        if (!minishell.token)
            continue;

        t_token *old = minishell.token;
        minishell.token = expand(&minishell);
        free_tokens(old);

        print_tokens(minishell.token);

        count_pipe(&minishell);

        if (minishell.cmd)
            free_commands(&minishell);

        minishell.cmd = malloc(sizeof(t_cmd) * (minishell.pipex_count + 1));
        if (!minishell.cmd)
            exit(EXIT_FAILURE);

        ft_bzero(minishell.cmd, sizeof(t_cmd) * (minishell.pipex_count + 1));
        put_token_to_commands(&minishell);
        print_commands(minishell.cmd);

        execute_command(&minishell, envp);

        free_tokens(minishell.token);
        minishell.token = NULL;

        free(minishell.promp_input);
        minishell.promp_input = NULL;
    }

    rl_clear_history();
    free_minishell(&minishell);
    return 0;
}


// void print_tokens(t_token *token)
// {
//     printf("\n🧩 Token List:\n");
//     while (token)
//     {
//         const char *type_str;
//         switch (token->type)
//         {
//             case WORD: type_str = "WORD"; break;
//             case PIPE: type_str = "PIPE"; break;
//             case REDIR_IN: type_str = "REDIR_IN"; break;
//             case REDIR_OUT: type_str = "REDIR_OUT"; break;
//             case REDIR_APPEND: type_str = "REDIR_APPEND"; break;
//             case HERE_DOC: type_str = "HERE_DOC"; break;
//             default: type_str = "UNKNOWN"; break;
//         }

//         printf("  [%s] \"%s\" (quote: %c)\n", type_str, token->value, token->quote ? token->quote : '-');
//         token = token->next;
//     }
//     printf("🔚 End of Tokens\n");
// }


// void print_commands(t_cmd *cmd)
// {
//     int i = 0;
//     printf("\n🚀 Command List:\n");
//     while (cmd)
//     {
//         printf("  Cmd[%d]:\n", i);
//         if (cmd->argv)
//         {
//             for (int j = 0; cmd->argv[j]; j++)
//                 printf("    argv[%d]: [%s]\n", j, cmd->argv[j]);
//         }
//         else
//             printf("    (no argv)\n");

//         // Input redirection
//         switch (cmd->in_type)
//         {
//             case REDIR_IN: printf("    input: REDIR_IN <%s>\n", cmd->input_file_name); break;
//             case HERE_DOC: printf("    input: HERE_DOC\n"); break;
//             case PIPE:     printf("    input: PIPE\n"); break;
//             default:       printf("    input: none\n"); break;
//         }

//         // Output redirection
//         switch (cmd->out_type)
//         {
//             case REDIR_OUT:    printf("    output: REDIR_OUT >%s\n", cmd->output_file_name); break;
//             case REDIR_APPEND: printf("    output: APPEND >>%s\n", cmd->output_file_name); break;
//             case PIPE:         printf("    output: PIPE\n"); break;
//             default:           printf("    output: none\n"); break;
//         }

//         cmd = cmd->next;
//         i++;
//     }
//     printf("🔚 End of Commands\n");
// }



// int main(int ac, char **av, char **envp)
// {
//     t_minishell minishell;

//     (void)ac;
//     (void)av;
//     init(&minishell);
//     minishell.envp = envp;
//     init_env_list(&minishell, envp);
//     while (1)
//     {
//         setup_signals();
//         init_shell(&minishell);

//         if (!minishell.promp_input || minishell.promp_input[0] == '\0')
//             continue;

//         minishell.token = tokenize(&minishell);
//         if (!minishell.token)
//             continue;

//         t_token *old = minishell.token;
//         minishell.token = expand(&minishell);
//         free_tokens(old);

//         print_tokens(minishell.token);

        
//         count_pipe(&minishell);
//         if (minishell.cmd)
//             free_commands(&minishell);
//         //minishell.cmd = malloc(sizeof(t_cmd) * (minishell.pipex_count + 1));
//         minishell.cmd = NULL;

        
//         if (!minishell.cmd)
//             exit(1);

//         put_token_to_commands(&minishell);

//         print_commands(minishell.cmd);
        
//         execute_command(&minishell, envp);


//         free_tokens(minishell.token);
//         minishell.token = NULL;

//         free(minishell.promp_input);
//         minishell.promp_input = NULL;
//     }
//     rl_clear_history();
//     free_minishell(&minishell);
//     return 0;
// }
