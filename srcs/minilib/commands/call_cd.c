#include "../../../include/minishell.h"

void call_cd(t_minishell *minishell, char **envp)
{
    char *path = minishell->cmd->argv[1];

    if (!path)
        path = get_env_value("HOME", minishell->env_list);

    if (chdir(path) != 0)
        perror("cd");
}

