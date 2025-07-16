#include "minishell.h"

void compare_commands (t_minishell *minishell, char **envp)
{
        if (!strcmp(minishell->cmd[0], "env"))
            call_env(minishell, envp);
        if (!strcmp(minishell->cmd[0], "echo"))
        {
            if (!strcmp(minishell->cmd[1], "-n"))
                call_echo(minishell, 1);
            else
                call_echo(minishell, 0);
        }
        else if (!strcmp(minishell->cmd[0], "pwd"))
            call_pwd(minishell);
        else if (!strcmp(minishell->cmd[0], "cd"))
            call_cd(minishell, envp);
        
        else if (!strcmp(minishell->cmd[0], "export"))
            call_export(envp, minishell->cmd[1]);
        else if (!strcmp(minishell->cmd[0], "unset"))
            call_unset(envp, minishell->cmd[1]);
        
        else
        {
            execvp(minishell->cmd[0], minishell->cmd);
            perror("execvp");
            exit(1);
        }
}