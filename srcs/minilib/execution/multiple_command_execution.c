#include "../../../include/minishell.h"

void multiple_command_execution(t_minishell *mini, char **envp)
{
    int count = mini->pipex_count + 1;
    pid_t pids[count];
    int status;

    ft_bzero(pids, sizeof(pids)); // Initialize all pids to 0

    execute_loop(mini, envp, pids);  // already defined in your file

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

// void execute_loop(t_minishell *mini, char **envp, pid_t *pids)
// {
//     int i = 0;
//     int fd[2];
//     int prev_fd = -1;
//     t_cmd *cmd = mini->cmd;

//     while (cmd)
//     {
//         if (pipe(fd) == -1)
//         {
//             perror("pipe");
//             return;
//         }

//         pid_t pid = fork();
//         if (pid < 0)
//         {
//             perror("fork");
//             return;
//         }
//         else if (pid == 0)
//         {
//             signal(SIGINT, SIG_DFL);
//             signal(SIGQUIT, SIG_DFL);

//             if (prev_fd != -1)
//             {
//                 dup2(prev_fd, STDIN_FILENO);
//                 close(prev_fd);
//             }

//             if (cmd->next)
//             {
//                 dup2(fd[1], STDOUT_FILENO);
//                 close(fd[1]);
//             }

//             close(fd[0]);
//             int is_last = (cmd->next == NULL);  // true if it's the last command
//             handle_redirections(cmd, prev_fd, fd, is_last);

//             if (is_builtin(cmd->argv[0]))
//                 execute_builtin_cmd(mini, cmd);
//             else
//             {
//                 char *path = resolve_cmd_path(cmd->argv[0], envp);
//                 if (!path)
//                 {
//                     fprintf(stderr, "minishell: command not found: %s\n", cmd->argv[0]);
//                     exit(127);
//                 }
//                 execve(path, cmd->argv, envp);
//                 perror("execve");
//                 exit(EXIT_FAILURE);
//             }
//         }
//         else
//         {
//             pids[i++] = pid;

//             if (prev_fd != -1)
//                 close(prev_fd);
//             close(fd[1]);
//             prev_fd = fd[0];
//         }

//         cmd = cmd->next;
//     }

//     if (prev_fd != -1)
//         close(prev_fd);
// }