#include "../../../include/minishell.h"

// Check if a string is a variable assignment (e.g., "var=value")
int is_variable_assignment(const char *str)
{
	if (!str)
		return 0;

	char *eq_pos = ft_strchr(str, '=');
	if (!eq_pos)
		return 0;

	// Extract key part before '='
	int key_len = eq_pos - str;
	if (key_len == 0)
		return 0;

	char *key = ft_substr(str, 0, key_len);
	if (!key)
		return 0;

	int is_valid = is_valid_identifier(key);
	free(key);

	return is_valid;
}

// Handle variable assignment by setting environment variable
void handle_variable_assignment(t_minishell *minishell, const char *assignment)
{
	char *eq_pos = ft_strchr(assignment, '=');
	if (!eq_pos)
		return;

	int key_len = eq_pos - assignment;
	char *key = ft_substr(assignment, 0, key_len);
	char *value = ft_strdup(eq_pos + 1);

	if (key && value)
	{
		minishell->env_list = set_env_value(minishell, key, value);
	}

	free(key);
	free(value);
}

int count_args_for_command(t_token *start)
{
	int count = 0;
	while (start && start->type != PIPE)
	{
		if (start->type == WORD)
			count++;
		start = start->next;
	}
	return count;
}

#define MAX_ARGS 512

// Merges argv tokens where an element ends with '=' followed by the next token.
// For example: ["_AA=", "ABC"] => ["_AA=ABC"]
static void merge_equal_sign_tokens(t_cmd *cmd)
{
	for (int j = 0; cmd->argv[j] && cmd->argv[j + 1]; j++)
	{
		char *arg = cmd->argv[j];
		size_t len = ft_strlen(arg);
		if (len > 0 && arg[len - 1] == '=')
		{
			char *merged = ft_strjoin(arg, cmd->argv[j + 1]);
			if (!merged)
			{
				write(2, "minishell: memory allocation failed\n", 36);
				// Ideally set a global minishell exit status or handle error
				exit(EXIT_FAILURE);
			}
			free(cmd->argv[j]);
			free(cmd->argv[j + 1]);
			cmd->argv[j] = merged;

			// Shift left remaining tokens
			int k = j + 1;
			while (cmd->argv[k + 1])
			{
				cmd->argv[k] = cmd->argv[k + 1];
				k++;
			}
			cmd->argv[k] = NULL;

			// Recheck current position after shift
			j--;
		}
	}
}

int put_token_to_commands(t_minishell *minishell)
{
	t_token *cur = minishell->token;
	int cmd_index = 0;
	int arg_index = 0;

	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		minishell->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
		if (!minishell->cmd[i].argv)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		for (int j = 0; j < MAX_ARGS; j++)
			minishell->cmd[i].argv[j] = NULL;
	}

	while (cur)
	{
		if (cur->type == PIPE)
		{
			minishell->cmd[cmd_index].argv[arg_index] = NULL;
			cmd_index++;
			arg_index = 0;

			if (cmd_index > minishell->pipex_count)
			{
				write(2, "minishell: internal error: too many commands\n", 45);
				minishell->exit_status = 1;
				exit(EXIT_FAILURE);
			}
		}
		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT || cur->type == REDIR_APPEND)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				write(2, "minishell: syntax error near redirection\n", 42);
				minishell->exit_status = 2;
				exit(EXIT_FAILURE);
			}

			const char *filename = cur->next->value;
			int fd = -1;

			if (cur->type == REDIR_OUT)
				fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else if (cur->type == REDIR_APPEND)
				fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
			else if (cur->type == REDIR_IN)
				fd = open(filename, O_RDONLY);

			if (fd < 0)
			{
				perror(filename);
				minishell->exit_status = 1;
				return -1;
			}
			close(fd);

			if (cur->type == REDIR_IN)
			{
				free(minishell->cmd[cmd_index].input_file_name);
				minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
				minishell->cmd[cmd_index].in_type = REDIR_IN;
			}
			else if (cur->type == REDIR_OUT)
			{
				free(minishell->cmd[cmd_index].output_file_name);
				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
				minishell->cmd[cmd_index].out_type = REDIR_OUT;
			}
			else if (cur->type == REDIR_APPEND)
			{
				free(minishell->cmd[cmd_index].output_file_name);
				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
				minishell->cmd[cmd_index].out_type = REDIR_APPEND;
			}

			cur = cur->next; // Skip filename token
		}
		else if (cur->type == HERE_DOC)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				write(2, "minishell: syntax error near redirection\n", 42);
				minishell->exit_status = 2;
				exit(EXIT_FAILURE);
			}
			const char *filename = cur->next->value;

			// Add heredoc to the list for multiple heredoc support
			add_heredoc_to_list(&minishell->cmd[cmd_index], filename, cur->next->quote);
			
			// Keep the last heredoc info for backward compatibility
			free(minishell->cmd[cmd_index].input_file_name);
			minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
			minishell->cmd[cmd_index].in_type = HERE_DOC;
			minishell->cmd[cmd_index].input_quote = cur->next->quote;

			cur = cur->next; // Skip filename token
		}
		else if (cur->type == WORD)
		{
			// Allow empty tokens for the first argument (command name)
			// to trigger proper "command not found" error
			if (cur->value[0] == '\0' && arg_index > 0)
			{
				cur = cur->next;
				continue;
			}
			// your variable assignment check here if needed

			minishell->cmd[cmd_index].argv[arg_index] = ft_strdup(cur->value);
			if (!minishell->cmd[cmd_index].argv[arg_index])
			{
				write(2, "minishell: memory allocation failed\n", 36);
				minishell->exit_status = 1; // General error
				exit(EXIT_FAILURE);
			}
			arg_index++;
		}

		cur = cur->next;
	}

	minishell->cmd[cmd_index].argv[arg_index] = NULL;

	// Call the new merge function on each command argv
	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		merge_equal_sign_tokens(&minishell->cmd[i]);
	}

	// Handle heredoc without command default to cat
	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		if (minishell->cmd[i].in_type == HERE_DOC &&
			(!minishell->cmd[i].argv || !minishell->cmd[i].argv[0]))
		{
			if (i == minishell->pipex_count)
			{
				minishell->cmd[i].argv[0] = ft_strdup("cat");
				if (!minishell->cmd[i].argv[0])
				{
					write(2, "minishell: memory allocation failed\n", 36);
					minishell->exit_status = 1;
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	return 0;
}

// int put_token_to_commands(t_minishell *minishell)
// {
// 	t_token *cur = minishell->token;
// 	int cmd_index = 0;
// 	int arg_index = 0;

// 	// init_cmd is already called in main, don't call it again

// 	for (int i = 0; i <= minishell->pipex_count; i++)
// 	{
// 		minishell->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
// 		if (!minishell->cmd[i].argv)
// 		{
// 			perror("malloc");
// 			exit(EXIT_FAILURE);
// 		}
// 		// Initialize all pointers to NULL
// 		for (int j = 0; j < MAX_ARGS; j++)
// 		{
// 			minishell->cmd[i].argv[j] = NULL;
// 		}
// 	}

// 	while (cur)
// 	{
// 		if (cur->type == PIPE)
// 		{
// 			minishell->cmd[cmd_index].argv[arg_index] = NULL;
// 			cmd_index++;
// 			arg_index = 0;

// 			if (cmd_index > minishell->pipex_count)
// 			{
// 				write(2, "minishell: internal error: too many commands\n", 45);
// 				minishell->exit_status = 1;  // General error
// 				exit(EXIT_FAILURE);
// 			}
// 		}
// 		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
// 				 cur->type == REDIR_APPEND)
// 		{
// 			if (!cur->next || cur->next->type != WORD)
// 			{
// 				write(2, "minishell: syntax error near redirection\n", 42);
// 				minishell->exit_status = 2;  // Incorrect usage
// 				exit(EXIT_FAILURE);
// 			}

// 			const char *filename = cur->next->value;

// 			// Open file to mimic Bash behavior (create/truncate/append)
// 			int fd = -1;
// 			if (cur->type == REDIR_OUT)
// 				fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 			else if (cur->type == REDIR_APPEND)
// 				fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 			else if (cur->type == REDIR_IN)
// 				fd = open(filename, O_RDONLY);

// 			if (fd < 0)
// 			{
// 				perror(filename);
// 				minishell->exit_status = 1;
// 				// printf("Remove the exit 1\n");
// 				// exit(EXIT_FAILURE);
// 				return -1;
// 			}
// 			if (fd >= 0)
// 				close(fd);

// 			// Apply redirection to the current command
// 			if (cur->type == REDIR_IN)
// 			{
// 				free(minishell->cmd[cmd_index].input_file_name);
// 				minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
// 				if (!minishell->cmd[cmd_index].input_file_name)
// 				{
// 					write(2, "minishell: memory allocation failed\n", 36);
// 					minishell->exit_status = 1;  // General error
// 					exit(EXIT_FAILURE);
// 				}
// 				minishell->cmd[cmd_index].in_type = REDIR_IN;
// 			}
// 			else if (cur->type == REDIR_OUT)
// 			{
// 				free(minishell->cmd[cmd_index].output_file_name);
// 				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
// 				if (!minishell->cmd[cmd_index].output_file_name)
// 				{
// 					write(2, "minishell: memory allocation failed\n", 36);
// 					minishell->exit_status = 1;  // General error
// 					exit(EXIT_FAILURE);
// 				}
// 				minishell->cmd[cmd_index].out_type = REDIR_OUT;
// 			}
// 			else if (cur->type == REDIR_APPEND)
// 			{
// 				free(minishell->cmd[cmd_index].output_file_name);
// 				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
// 				if (!minishell->cmd[cmd_index].output_file_name)
// 				{
// 					write(2, "minishell: memory allocation failed\n", 36);
// 					minishell->exit_status = 1;  // General error
// 					exit(EXIT_FAILURE);
// 				}
// 				minishell->cmd[cmd_index].out_type = REDIR_APPEND;
// 			}

// 			cur = cur->next; // skip the filename
// 		}
// 		else if (cur->type == HERE_DOC)
// 		{
// 			if (!cur->next || cur->next->type != WORD)
// 			{
// 				write(2, "minishell: syntax error near redirection\n", 42);
// 				minishell->exit_status = 2;  // Incorrect usage
// 				exit(EXIT_FAILURE);
// 			}

// 			const char *filename = cur->next->value;

// 			// For multiple heredocs, only keep the last one
// 			// This ensures that only the final delimiter is used for the heredoc
// 			// The heredoc processing will read all content until the final delimiter
// 			free(minishell->cmd[cmd_index].input_file_name);
// 			minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
// 			if (!minishell->cmd[cmd_index].input_file_name)
// 			{
// 				write(2, "minishell: memory allocation failed\n", 36);
// 				minishell->exit_status = 1;  // General error
// 				exit(EXIT_FAILURE);
// 			}
// 			minishell->cmd[cmd_index].in_type = HERE_DOC;
// 			minishell->cmd[cmd_index].input_quote = cur->next->quote;

// 			cur = cur->next; // skip the filename
// 		}
// 		else if (cur->type == WORD)
// 		{
// 			// Check for variable assignment (only as first argument/command)
// 			if (arg_index == 0 && is_variable_assignment(cur->value))
// 			{
// 				handle_variable_assignment(minishell, cur->value);
// 				// Don't add this to argv, it's a variable assignment not a command
// 				// Continue to next token without incrementing arg_index
// 			}
// 			else
// 			{
// 				minishell->cmd[cmd_index].argv[arg_index] = ft_strdup(cur->value);
// 				if (!minishell->cmd[cmd_index].argv[arg_index])
// 				{
// 					write(2, "minishell: memory allocation failed\n", 36);
// 					minishell->exit_status = 1;  // General error
// 					exit(EXIT_FAILURE);
// 				}
// 				arg_index++;
// 			}
// 		}
// 		cur = cur->next;
// 	}

// 	// Null-terminate last command argv
// 	minishell->cmd[cmd_index].argv[arg_index] = NULL;

// 	// Handle heredoc without command (default to cat)
// 	for (int i = 0; i <= minishell->pipex_count; i++)
// 	{
// 		if (minishell->cmd[i].in_type == HERE_DOC &&
// 			(!minishell->cmd[i].argv || !minishell->cmd[i].argv[0]))
// 		{
// 			// Add cat as default command only if there's no pipe after this command
// 			// or if this is the last command in the pipeline
// 			if (i == minishell->pipex_count)
// 			{
// 				minishell->cmd[i].argv[0] = ft_strdup("cat");
// 				if (!minishell->cmd[i].argv[0])
// 				{
// 					write(2, "minishell: memory allocation failed\n", 36);
// 					minishell->exit_status = 1;  // General error
// 					exit(EXIT_FAILURE);
// 				}
// 			}
// 			// If there's a pipe after this command, leave it as empty command
// 			// The heredoc content will flow to the next command
// 		}
// 	}
// 	return 0; // Success
// }
