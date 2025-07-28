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