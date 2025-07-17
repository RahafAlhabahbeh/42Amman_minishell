#include "../../../include/minishell.h"
/*
void call_echo(t_minishell *minishell, int op)
{
    int i = (op == 1) ? 2 : 1;  // skip -n if needed

    while (minishell->cmd->argv[i])
    {
        printf("%s", minishell->cmd->argv[i]);
        if (minishell->cmd->argv[i + 1])
            printf(" ");
        i++;
    }
    if (!op)
        printf("\n");
}
*/
void call_echo(char **argv)
{
    int i = 1;
    int newline = 1;
    if (argv[1] && strcmp(argv[1], "-n") == 0)
    {
        newline = 0;
        i++;
    }

    while (argv[i])
    {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}


