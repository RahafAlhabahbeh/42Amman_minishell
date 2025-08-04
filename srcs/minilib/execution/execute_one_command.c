#include "../../../include/minishell.h"

void execute_one_command(t_minishell *mini, char **envp)
{
    fprintf(stdout, "Executing one command...\n");
    t_cmd *cmd = mini->cmd;

    if (!cmd || !cmd->argv || !cmd->argv[0])
        return;

    // Handle heredoc
    if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
        return;

    // Handle built-in without fork
    if (is_builtin(cmd->argv[0]) &&
        is_str_in_set(cmd->argv[0], (char *[]){"export", "unset", "cd", "exit", NULL}))
    {
        handle_redirections(cmd, -1, NULL, 1);
        execute_builtin_cmd(mini, cmd);
        return;
    }

    // Fork and execute in child
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        return;
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        handle_redirections(cmd, -1, NULL, 1);
        if (is_builtin(cmd->argv[0]))
        {
            execute_builtin_cmd(mini, cmd);
            exit(0);
        }
        char *path = resolve_cmd_path(cmd->argv[0], envp);
        if (!path)
        {
            fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
            exit(127);
        }
        execve(path, cmd->argv, envp);
        perror("execve");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            mini->exit_status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            mini->exit_status = 128 + WTERMSIG(status);
    }

    cleanup_heredoc_files(mini);
}
int is_one_command(t_minishell *mini)
{
    return (mini->pipex_count == 0);
}
