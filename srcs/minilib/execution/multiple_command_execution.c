#include "../../../include/minishell.h"

void multiple_command_execution(t_minishell *mini, char **envp)
{
    int count = mini->pipex_count + 1;
    pid_t pids[count];
    int status;
    ft_bzero(pids, sizeof(pids));
    execute_loop(mini, envp, pids);
    for (int i = 0; i < count; i++)
    {
        if (pids[i] > 0)
        {
            waitpid(pids[i], &status, 0);
            if (WIFEXITED(status))
                mini->exit_status = WEXITSTATUS(status);
            else if (WIFSIGNALED(status))
                mini->exit_status = 128 + WTERMSIG(status);
        }
    }
    cleanup_heredoc_files(mini);
}