#include "../../../include/minishell.h"

void multiple_command_execution(t_minishell *mini, char **envp)
{
    int count = mini->pipex_count + 1;
    pid_t pids[count];
    int status;
    
    // Initialize pids array to -1
    for (int i = 0; i < count; i++)
        pids[i] = -1;
        
    execute_loop(mini, envp, pids);
    set_child_process_flag(1);  // Set flag before waiting for children
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
        else if (pids[i] == 0)
        {
            // This is a child process, it should have already exited
            // or we're in the wrong context
            continue;
        }
    }
    set_child_process_flag(0);  // Clear flag after all children are done
    cleanup_heredoc_files(mini);
}