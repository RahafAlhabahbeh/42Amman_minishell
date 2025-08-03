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
    close(STDIN_FILENO);
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
    
    char *line;
    while (1)
    {
        line = readline("> ");
        if (!line || g_heredoc_signal)
        {
            if (!line && !g_heredoc_signal)
            {
                write(2, "bash: warning: here-document at line ", 35);
                write(2, "delimited by end-of-file (wanted `", 33);
                write(2, clean_delimiter, ft_strlen(clean_delimiter));
                write(2, "')\n", 3);
            }
            free(line);
            break;
        }
        
        // Check for exact delimiter match (including quotes if present)
        if (ft_strcmp(line, clean_delimiter) == 0)
        {
            free(line);
            break;
        }
        
        char *expanded_line = expand_heredoc_line(mini, line, expand_vars);
        if (expanded_line)
        {
            write(temp_fd, expanded_line, ft_strlen(expanded_line));
            write(temp_fd, "\n", 1);
            free(expanded_line);
        }
        free(line);
    }
    
    // Restore original signal handler
    signal(SIGINT, old_sigint);
    
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
    
    // Create a delimiter string with quotes if needed
    char delimiter_with_quotes[256];
    if (cmd->input_quote)
    {
        sprintf(delimiter_with_quotes, "%c%s%c", cmd->input_quote, cmd->input_file_name, cmd->input_quote);
    }
    else
    {
        ft_strlcpy(delimiter_with_quotes, cmd->input_file_name, sizeof(delimiter_with_quotes));
    }
    
    int heredoc_fd = create_heredoc_temp_file(mini, delimiter_with_quotes, &cmd->heredoc_temp_file);
    if (heredoc_fd < 0)
    {
        mini->exit_status = 1;
        return -1;
    }
    
    // Replace the heredoc filename with the temp file descriptor
    free(cmd->input_file_name);
    char fd_str[16];
    sprintf(fd_str, "%d", heredoc_fd);
    cmd->input_file_name = ft_strdup(fd_str);
    cmd->in_type = REDIR_IN; // Change to regular input redirection
    
    return 0;
}

void redirect_heredoc_input(const char *fd_str)
{
    int fd = ft_atoi(fd_str);
    if (fd >= 0)
    {
        dup2(fd, STDIN_FILENO);
        // Don't close fd here, it will be closed in cleanup
    }
}

void cleanup_heredoc_files(t_minishell *mini)
{
    t_cmd *cmd = mini->cmd;
    for (int i = 0; i <= mini->pipex_count; i++)
    {
        if (cmd[i].input_file_name && ft_isdigit(cmd[i].input_file_name[0]))
        {
            int fd = ft_atoi(cmd[i].input_file_name);
            if (fd >= 0)
            {
                close(fd);
            }
        }
        if (cmd[i].heredoc_temp_file)
        {
            unlink(cmd[i].heredoc_temp_file);
            free(cmd[i].heredoc_temp_file);
            cmd[i].heredoc_temp_file = NULL;
        }
    }
} 