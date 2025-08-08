#include "../../../include/minishell.h"

void execute_one_command(t_minishell *mini, char **envp)
{
    // fprintf(stdout, "Executing one command...\n");
    // fprintf(stderr, "Executing one command...\n");
    t_cmd *cmd = mini->cmd;

    if (!cmd || !cmd->argv[0])
        return;

    if (cmd->argv[0][0] == '\0')
    {
        fprintf(stderr, "bash: %s: command not found\n", cmd->argv[0]);
        mini->exit_status = 127; // Command not found
        return;
    }

    // Handle heredoc
    if (cmd->in_type == HERE_DOC && handle_heredoc(mini, cmd) < 0)
        return;

    // Handle built-in without fork
    if (is_builtin(cmd->argv[0]) &&
        is_str_in_set(cmd->argv[0], (char *[]){"export", "unset", "cd", "exit", NULL}))
    {

        save_original_fds(cmd);
        if (handle_redirections(cmd, -1, NULL, 1) < 0)
        {
            restore_original_fds(cmd);
            mini->exit_status = 1;
            return;
        }
        execute_builtin_cmd(mini, cmd);
        restore_original_fds(cmd);
        return;
    }

    // Fork and execute in child
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork");
        mini->exit_status = 1; // General error
        return;
    }
    else if (pid == 0)
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        if (handle_redirections(cmd, -1, NULL, 1) < 0)
            exit(1);
        if (is_builtin(cmd->argv[0]))
        {
            execute_builtin_cmd(mini, cmd);
            exit(0);
        }
        char *path = resolve_cmd_path(cmd->argv[0], envp);
        if (!path)
        {
            write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
            write(2, ": command not found\n", 20);
            exit(127);
        }
        execve(path, cmd->argv, envp);
        perror("execve");
        exit(126); // Command invoked cannot execute
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
