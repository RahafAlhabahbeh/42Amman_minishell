#include "../../../include/minishell.h"

void redirect_input(const char *file)
{
    int fd = open(file, O_RDONLY);
    if (fd < 0)
    {
        perror("open input");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirect_output(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open output");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void redirect_output_append(const char *file)
{
    int fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd < 0)
    {
        perror("open output append");
        exit(EXIT_FAILURE);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}

void handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last, t_minishell *mini)
{
    (void)mini; // Suppress unused parameter warning
    // First, handle input redirection (this takes precedence over pipe input)
    if (cmd->input_file_name)
    {
        // Check if it's a heredoc file descriptor (numeric string)
        if (ft_isdigit(cmd->input_file_name[0]))
            redirect_heredoc_input(cmd->input_file_name);
        else
            redirect_input(cmd->input_file_name);
    }
    else if (prev_fd != -1)
    {
        // Only use pipe input if no file redirection
        dup2(prev_fd, STDIN_FILENO);
        close(prev_fd);
    }   
    
    // Then handle output redirection (this takes precedence over pipe output)
    if (cmd->output_file_name)
    {
        if (cmd->out_type == REDIR_APPEND)
            redirect_output_append(cmd->output_file_name);
        else
            redirect_output(cmd->output_file_name);
    }
    else if (!is_last)
    {
        // Only use pipe output if no file redirection
        close(pipe_fds[0]);
        dup2(pipe_fds[1], STDOUT_FILENO);
        close(pipe_fds[1]);
    }
}
