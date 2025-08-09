#include "../../../include/minishell.h"

void execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last, char **envp)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);

    // Heredoc is already handled in parent process

    save_original_fds(cmd);
    handle_redirections(cmd, prev_fd, pipe_fds, is_last);

    if (is_builtin(cmd->argv[0]))
    {
        execute_builtin_cmd(mini, cmd);
        restore_original_fds(cmd);
        exit(0);
    }

    char *path = resolve_cmd_path(cmd->argv[0], mini);
    if (!path)
    {
        write(2, "minishell: command not found: ", 30);
        write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
        write(2, "\n", 1);
        exit(127);
    }

    execve(path, cmd->argv, envp);
    perror("execve");
    exit(EXIT_FAILURE);
}


// void execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
//                                   int *pipe_fds, int is_last, char **envp)
// {
//     signal(SIGINT, SIG_DFL);
//     signal(SIGQUIT, SIG_DFL);

//     // Handle heredoc if needed
//     if (cmd->in_type == HERE_DOC)
//     {
//         if (handle_heredoc(mini, cmd) < 0)
//             exit(1);
//     }
//     // save_original_fds(cmd);
//     if (is_builtin(cmd->argv[0]))
//     {
//         handle_redirections(cmd, prev_fd, pipe_fds, is_last, mini);
//         execute_builtin_cmd(mini, cmd);
//         // restore_original_fds(cmd);
//         free_commands(mini->cmd, mini->pipex_count);
//         free_tokens(mini->token);
//         free(mini->promp_input);
//         exit(0);
//     }

//     handle_redirections(cmd, prev_fd, pipe_fds, is_last, mini);
//     char *path = resolve_cmd_path(cmd->argv[0], mini);
//     if (!path)
//     {
//         write(2, "minishell: command not found: ", 30);
//         write(2, cmd->argv[0], ft_strlen(cmd->argv[0]));
//         write(2, "\n", 1);
//         exit(127);
//     }
//     execve(path, cmd->argv, envp);
//     perror("execve");
//     exit(EXIT_FAILURE);
// }