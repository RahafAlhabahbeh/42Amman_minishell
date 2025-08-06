/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/31 01:11:13 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int g_heredoc_signal = 0;

void handle_heredoc_sigint(int sig)
{
    (void)sig;
    g_heredoc_signal = 1;
    write(1, "\n", 1);
    // Set the flag and let the main loop handle the break
}

static char *expand_heredoc_line(t_minishell *mini, char *line, int expand_vars)
{
    if (!expand_vars || !line)
        return ft_strdup(line);
    
    char *result = malloc(1024);
    if (!result)
        return NULL;
    
    int i = 0, j = 0;
    while (line[i])
    {
        if (line[i] == '$' && line[i + 1] && 
            (ft_isalnum(line[i + 1]) || line[i + 1] == '_' || line[i + 1] == '?'))
        {
            i++; // skip $
            char var_name[256] = {0};
            int k = 0;
            
            if (line[i] == '?')
            {
                var_name[k++] = '?';
                i++;
            }
            else
            {
                while (line[i] && (ft_isalnum(line[i]) || line[i] == '_'))
                {
                    var_name[k++] = line[i++];
                }
            }
            var_name[k] = '\0';
            
            char *var_value = NULL;
            if (ft_strcmp(var_name, "?") == 0)
            {
                char exit_str[16];
                sprintf(exit_str, "%d", mini->exit_status);
                var_value = ft_strdup(exit_str);
            }
            else
            {
                var_value = get_value_env(mini, var_name);
                if (var_value)
                    var_value = ft_strdup(var_value);
            }
            
            if (var_value)
            {
                ft_strlcat(result, var_value, 1024);
                j += ft_strlen(var_value);
                free(var_value);
            }
        }
        else
        {
            result[j++] = line[i++];
        }
    }
    result[j] = '\0';
    return result;
}

static int is_quoted_delimiter(const char *delimiter)
{
    return (delimiter[0] == '\'' || delimiter[0] == '"');
}

static char *remove_quotes(const char *delimiter)
{
    int len = ft_strlen(delimiter);
    if (len >= 2 && ((delimiter[0] == '\'' && delimiter[len - 1] == '\'') ||
                     (delimiter[0] == '"' && delimiter[len - 1] == '"')))
    {
        char *unquoted = malloc(len - 1);
        if (!unquoted)
            return NULL;
        ft_strlcpy(unquoted, delimiter + 1, len - 1);
        return unquoted;
    }
    return ft_strdup(delimiter);
}



static int create_heredoc_temp_file(t_minishell *mini, const char *delimiter, char **temp_filename_ptr)
{
    char temp_filename[256];
    static int heredoc_counter = 0;
    
    sprintf(temp_filename, "/tmp/heredoc_%d_%d", getpid(), heredoc_counter++);
    *temp_filename_ptr = ft_strdup(temp_filename);
    
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (temp_fd < 0)
    {
        perror("minishell: heredoc temp file creation failed");
        return -1;
    }
    
    // Determine if we should expand variables
    int expand_vars = !is_quoted_delimiter(delimiter);
    char *clean_delimiter = remove_quotes(delimiter);
    if (!clean_delimiter)
    {
        close(temp_fd);
        unlink(temp_filename);
        return -1;
    }
    
    // Set up signal handler for heredoc
    void (*old_sigint)(int) = signal(SIGINT, handle_heredoc_sigint);
    g_heredoc_signal = 0;
    set_child_process_flag(1);  // Set flag during heredoc input
    
    char *line;
    int temp_content_fd = -1;
    char temp_content_filename[256];
    
    // Create a temporary file to store all content first
    sprintf(temp_content_filename, "/tmp/heredoc_content_%d_%d", getpid(), heredoc_counter);
    temp_content_fd = open(temp_content_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (temp_content_fd < 0)
    {
        close(temp_fd);
        unlink(temp_filename);
        free(clean_delimiter);
        return -1;
    }
    
    // Read all content until the final delimiter
    while (!g_heredoc_signal)
    {
        write(1, "> ", 2);
        line = NULL;
        size_t line_size = 0;
        ssize_t read_size = getline(&line, &line_size, stdin);
        
        if (read_size == -1 || g_heredoc_signal)
        {
            if (read_size == -1 && !g_heredoc_signal)
            {
                write(2, "bash: warning: here-document at line ", 35);
                write(2, "delimited by end-of-file (wanted '", 33);
                write(2, clean_delimiter, ft_strlen(clean_delimiter));
                write(2, "')\n", 3);
            }
            free(line);
            break;
        }
        
        // Remove newline from the end
        if (line && line[read_size - 1] == '\n')
            line[read_size - 1] = '\0';
        
        // Check for exact delimiter match
        if (ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break;
        }
        
        // Store all content in temporary file
        char *expanded_line = expand_heredoc_line(mini, line, expand_vars);
        if (expanded_line)
        {
            write(temp_content_fd, expanded_line, ft_strlen(expanded_line));
            write(temp_content_fd, "\n", 1);
            free(expanded_line);
        }
        free(line);
    }
    
    // Close temporary content file
    close(temp_content_fd);
    
    // Simply copy the content to the final temp file
    temp_content_fd = open(temp_content_filename, O_RDONLY);
    if (temp_content_fd >= 0)
    {
        char buffer[1024];
        ssize_t bytes_read;
        
        while ((bytes_read = read(temp_content_fd, buffer, sizeof(buffer))) > 0)
        {
            write(temp_fd, buffer, bytes_read);
        }
        
        close(temp_content_fd);
        unlink(temp_content_filename);
    }
    
    // Restore original signal handler
    signal(SIGINT, old_sigint);
    set_child_process_flag(0);  // Clear flag after heredoc processing
    
    close(temp_fd);
    free(clean_delimiter);
    
    if (g_heredoc_signal)
    {
        unlink(temp_filename);
        return -1;
    }
    
    return open(temp_filename, O_RDONLY);
}



int handle_heredoc(t_minishell *mini, t_cmd *cmd)
{
    if (cmd->in_type != HERE_DOC || !cmd->input_file_name)
        return 0;
    
    // Check if there are multiple heredocs in the command
    // For now, we'll use the current delimiter and let the command parsing handle multiple heredocs
    // by only keeping the last one
    
    // Pass the delimiter as-is, let create_heredoc_temp_file handle quote processing
    int heredoc_fd = create_heredoc_temp_file(mini, cmd->input_file_name, &cmd->heredoc_temp_file);
    if (heredoc_fd < 0)
    {
        mini->exit_status = 1;
        return -1;
    }
    
    // Store the file descriptor directly instead of converting to string
    cmd->heredoc_fd = heredoc_fd;
    cmd->in_type = REDIR_IN; // Change to regular input redirection
    
    return 0;
}

void redirect_heredoc_input(t_cmd *cmd)
{
    if (!cmd || cmd->heredoc_fd < 0)
        return;
        
    dup2(cmd->heredoc_fd, STDIN_FILENO);
    close(cmd->heredoc_fd);
    cmd->heredoc_fd = -1;
}

void cleanup_heredoc_files(t_minishell *mini)
{
    if (!mini || !mini->cmd)
        return;
        
    t_cmd *cmd = mini->cmd;
    for (int i = 0; i <= mini->pipex_count; i++)
    {
        if (cmd[i].heredoc_fd >= 0)
        {
            close(cmd[i].heredoc_fd);
            cmd[i].heredoc_fd = -1;
        }
        if (cmd[i].heredoc_temp_file)
        {
            unlink(cmd[i].heredoc_temp_file);
            free(cmd[i].heredoc_temp_file);
            cmd[i].heredoc_temp_file = NULL;
        }
    }
} 