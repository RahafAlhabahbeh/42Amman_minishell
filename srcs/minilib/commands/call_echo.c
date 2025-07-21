#include "../../../include/minishell.h"

void call_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
    {
        int j = 2;
        while (argv[i][j] == 'n') j++;
        if (argv[i][j] == '\0')
        {
            newline = 0;
            i++;
        }
        else
            break;
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