#include "../../../include/minishell.h"

int redirect_input(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("open input");
        return -1;
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
    return 0;
}

int redirect_output(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open output");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

int redirect_output_append(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("open output append");
        return -1;
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return 0;
}

void save_original_fds(t_cmd *cmd)
{
    cmd->original_stdin = dup(STDIN_FILENO);
    cmd->original_stdout = dup(STDOUT_FILENO);
}



void restore_original_fds(t_cmd *cmd)
{
    if (cmd->original_stdin != -1)
        dup2(cmd->original_stdin, STDIN_FILENO);
    if (cmd->original_stdout != -1)
        dup2(cmd->original_stdout, STDOUT_FILENO);
    if (cmd->original_stdin != -1)
        close(cmd->original_stdin);
    if (cmd->original_stdout != -1)
        close(cmd->original_stdout);
}


// int handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last)
// {
//     if (cmd->input_file_name)
//         redirect_input(cmd->input_file_name);
//     else if (prev_fd != -1)
//         dup2(prev_fd, STDIN_FILENO);

//     if (cmd->output_file_name)
//     {
//         if (cmd->out_type == REDIR_APPEND)
//             redirect_output_append(cmd->output_file_name);
//         else
//             redirect_output(cmd->output_file_name);
//     }
//     else if (!is_last)
//     {
//         close(pipe_fds[0]);
//         dup2(pipe_fds[1], STDOUT_FILENO);
//         close(pipe_fds[1]);
//     }
// }
int handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last)
{
    // Handle input redirection
    if (cmd->heredoc_fd >= 0)
    {
        // Heredoc takes precedence over other input redirections
        redirect_heredoc_input(cmd);
    }
    else if (cmd->input_file_name)
    {
        if (redirect_input(cmd->input_file_name) < 0)
            return -1;
    }
    else if (prev_fd != -1)
    {
        dup2(prev_fd, STDIN_FILENO);
    }

    // Handle output redirection
    if (cmd->output_file_name)
    {
        if (cmd->out_type == REDIR_APPEND)
        {
            if (redirect_output_append(cmd->output_file_name) < 0)
                return -1;
        }
        else
        {
            if (redirect_output(cmd->output_file_name) < 0)
                return -1;
        }
    }
    else if (!is_last)
    {
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
    }

    return 0;
}
