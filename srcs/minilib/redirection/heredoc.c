/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/13 18:19:13 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// Use external signal handling from signals.c
extern int	check_sigint_received(void);

// Custom getline implementation using only allowed functions
static ssize_t	ft_getline(char **lineptr, size_t *n, int fd)
{
	char		c;
	ssize_t		bytes_read;
	size_t		pos;
	size_t		size;

	pos = 0;
	size = 0;
	if (!lineptr || !n)
		return (-1);
	// Initialize buffer if needed
	if (!*lineptr || *n == 0)
	{
		size = 128;
		*lineptr = malloc(size);
		if (!*lineptr)
			return (-1);
		*n = size;
	}
	while ((bytes_read = read(fd, &c, 1)) > 0)
	{
		// Expand buffer if needed
		if (pos >= *n - 1)
		{
			size = *n * 2;
			char *new_ptr = malloc(size);
			if (!new_ptr)
				return (-1);
			// Copy old content
			for (size_t i = 0; i < pos; i++)
				new_ptr[i] = (*lineptr)[i];
			free(*lineptr);
			*lineptr = new_ptr;
			*n = size;
		}
		(*lineptr)[pos++] = c;
		if (c == '\n')
			break;
	}
	if (bytes_read < 0)
		return (-1);
	if (pos == 0 && bytes_read == 0)
		return (-1);
	(*lineptr)[pos] = '\0';
	return (pos);
}

void handle_heredoc_sigint(int sig)
{
    // Set the global signal flag so check_sigint_received() can detect it
    g_received_signal = sig;
    write(1, "\n", 1);
}

static char *expand_heredoc_line(t_minishell *mini, char *line, int expand_vars)
{
    if (!expand_vars || !line)
        return ft_strdup(line);
    
    char *result = malloc(1024);
    if (!result)
        return NULL;
    
    result[0] = '\0';  // Initialize to empty string
    int i = 0, j = 0;
    while (line[i])
    {
        if (line[i] == '$' && line[i + 1] &&
			(ft_isalnum(line[i + 1]) || line[i + 1] == '_'
				|| line[i + 1] == '?'))
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
                // Simple int to string conversion
                int num = mini->exit_status;
                int i = 0;
                if (num == 0)
                {
                    exit_str[0] = '0';
                    exit_str[1] = '\0';
                }
                else
                {
                    while (num > 0)
                    {
                        exit_str[i++] = (num % 10) + '0';
                        num /= 10;
                    }
                    exit_str[i] = '\0';
                    // Reverse string
                    int start = 0, end = i - 1;
                    while (start < end)
                    {
                        char temp = exit_str[start];
                        exit_str[start] = exit_str[end];
                        exit_str[end] = temp;
                        start++;
                        end--;
                    }
                }
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
                int var_len = ft_strlen(var_value);
                if (j + var_len < 1023)
                {
                    ft_strlcpy(result + j, var_value, 1024 - j);
                    j += var_len;
                }
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

static int create_heredoc_temp_file_with_quote(t_minishell *mini,
	const char *delimiter, char **temp_filename_ptr, char quote_char)
{
    char temp_filename[256];
    static int heredoc_counter = 0;
    
    // Build filename manually without sprintf
    ft_strlcpy(temp_filename, "/tmp/heredoc_", sizeof(temp_filename));
    // Convert getpid() to string and append
    char *pid_str = ft_itoa(getpid());
    if (!pid_str)
        return (-1);
    
    ft_strlcat(temp_filename, pid_str, sizeof(temp_filename));
    ft_strlcat(temp_filename, "_", sizeof(temp_filename));
    free(pid_str);
    
    // Convert counter to string and append
    char *counter_str = ft_itoa(heredoc_counter++);
    if (!counter_str)
        return (-1);
    
    ft_strlcat(temp_filename, counter_str, sizeof(temp_filename));
    free(counter_str);
    *temp_filename_ptr = ft_strdup(temp_filename);
    
    int temp_fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (temp_fd < 0)
    {
        perror("minishell: heredoc temp file creation failed");
        return -1;
    }
    
    // Determine if we should expand variables based on quote information
    int expand_vars = (quote_char != '\'' && quote_char != '"');
    // Use delimiter as-is since quotes are already handled by tokenizer
    char *clean_delimiter = ft_strdup(delimiter);
    if (!clean_delimiter)
    {
        close(temp_fd);
        unlink(temp_filename);
        return -1;
    }
    
    // Set up signal handler for heredoc
    void (*old_sigint)(int) = signal(SIGINT, handle_heredoc_sigint);
    
    char *line;
    int temp_content_fd = -1;
    char temp_content_filename[256];
    
    // Create a temporary file to store all content first
    // Build filename manually without sprintf
    ft_strlcpy(temp_content_filename, "/tmp/heredoc_content_", sizeof(temp_content_filename));
    // Convert getpid() to string and append
    char *pid_str2 = ft_itoa(getpid());
    if (!pid_str2)
    {
        close(temp_fd);
        unlink(temp_filename);
        free(clean_delimiter);
        return (-1);
    }
    
    ft_strlcat(temp_content_filename, pid_str2, sizeof(temp_content_filename));
    ft_strlcat(temp_content_filename, "_", sizeof(temp_content_filename));
    free(pid_str2);
    
    // Convert counter to string and append
    char *counter_str2 = ft_itoa(heredoc_counter);
    if (!counter_str2)
    {
        close(temp_fd);
        unlink(temp_filename);
        free(clean_delimiter);
        return (-1);
    }
    
    ft_strlcat(temp_content_filename, counter_str2, sizeof(temp_content_filename));
    free(counter_str2);
    temp_content_fd = open(temp_content_filename,
		O_WRONLY | O_CREAT | O_TRUNC, 0600);
    if (temp_content_fd < 0)
    {
        close(temp_fd);
        unlink(temp_filename);
        free(clean_delimiter);
        return -1;
    }
    
    // Read all content until the final delimiter
    while (!check_sigint_received())
    {
        write(1, "> ", 2);
        line = NULL;
        size_t line_size = 0;
        ssize_t read_size = ft_getline(&line, &line_size, 0); // 0 = STDIN_FILENO
        
        int sigint_received = check_sigint_received();
        if (read_size == -1 || sigint_received)
        {
            if (read_size == -1 && !sigint_received)
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
    
    close(temp_fd);
    free(clean_delimiter);
    
    // Check if SIGINT was received during heredoc processing
    if (check_sigint_received())
    {
        unlink(temp_filename);
        unlink(temp_content_filename);
        return -1;
    }
    
    return open(temp_filename, O_RDONLY);
}



int handle_heredoc(t_minishell *mini, t_cmd *cmd)
{
    if (cmd->in_type != HERE_DOC)
        return 0;
    
    // Handle multiple heredocs if they exist
    if (cmd->heredoc_list)
    {
        return process_multiple_heredocs(mini, cmd);
    }
    
    // Handle single heredoc (fallback for backward compatibility)
    if (!cmd->input_file_name)
        return 0;
    
    int heredoc_fd = create_heredoc_temp_file_with_quote(mini, cmd->input_file_name, 
                                                        &cmd->heredoc_temp_file, cmd->input_quote);
    if (heredoc_fd < 0)
    {
        mini->exit_status = 1;
        return -1;
    }
    
    cmd->heredoc_fd = heredoc_fd;
    cmd->in_type = REDIR_IN;
    
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

void add_heredoc_to_list(t_cmd *cmd, const char *delimiter, char quote)
{
    t_heredoc *new_heredoc = malloc(sizeof(t_heredoc));
    if (!new_heredoc)
        return;
    
    new_heredoc->delimiter = ft_strdup(delimiter);
    new_heredoc->quote = quote;
    new_heredoc->next = NULL;
    
    if (!cmd->heredoc_list)
    {
        cmd->heredoc_list = new_heredoc;
    }
    else
    {
        t_heredoc *current = cmd->heredoc_list;
        while (current->next)
            current = current->next;
        current->next = new_heredoc;
    }
}

void free_heredoc_list(t_heredoc *list)
{
    t_heredoc *current = list;
    t_heredoc *next;
    
    while (current)
    {
        next = current->next;
        free(current->delimiter);
        free(current);
        current = next;
    }
}

int process_multiple_heredocs(t_minishell *mini, t_cmd *cmd)
{
    t_heredoc *current = cmd->heredoc_list;
    char *last_temp_file = NULL;
    int last_fd = -1;
    
    if (!current)
        return 0;
    
    // Process each heredoc in sequence
    while (current)
    {
        // Close previous temp file if exists
        if (last_fd >= 0)
        {
            close(last_fd);
            if (last_temp_file)
                unlink(last_temp_file);
        }
        
        // Create temp file for this heredoc
        last_fd = create_heredoc_temp_file_with_quote(mini, current->delimiter, 
                                                     &last_temp_file, current->quote);
        if (last_fd < 0)
        {
            if (last_temp_file)
                free(last_temp_file);
            return -1;
        }
        
        // If not the last heredoc, consume and discard the input
        if (current->next)
        {
            close(last_fd);
            unlink(last_temp_file);
            free(last_temp_file);
            last_temp_file = NULL;
            last_fd = -1;
        }
        
        current = current->next;
    }
    
    // Store the last heredoc result
    if (last_fd >= 0)
    {
        cmd->heredoc_fd = last_fd;
        cmd->heredoc_temp_file = last_temp_file;
        cmd->in_type = REDIR_IN;
        return 0;
    }
    
    return -1;
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
        if (cmd[i].heredoc_list)
        {
            free_heredoc_list(cmd[i].heredoc_list);
            cmd[i].heredoc_list = NULL;
        }
    }
}
