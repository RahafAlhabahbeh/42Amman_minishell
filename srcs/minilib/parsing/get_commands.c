#include "../../../include/minishell.h"

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

void put_token_to_commands(t_minishell *minishell)
{
	t_token *cur = minishell->token;
	int cmd_index = 0;
	int arg_index = 0;

	// init_cmd is already called in main, don't call it again

	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		minishell->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
		if (!minishell->cmd[i].argv)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
		}
		// Initialize all pointers to NULL
		for (int j = 0; j < MAX_ARGS; j++)
		{
			minishell->cmd[i].argv[j] = NULL;
		}
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
				minishell->exit_status = 1;  // General error
				exit(EXIT_FAILURE);
			}
		}
		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
				 cur->type == REDIR_APPEND)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				write(2, "minishell: syntax error near redirection\n", 42);
				minishell->exit_status = 2;  // Incorrect usage
				exit(EXIT_FAILURE);
			}

			const char *filename = cur->next->value;

			// Open file to mimic Bash behavior (create/truncate/append)
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
				//printf("Remove the exit 1\n");
				// exit(EXIT_FAILURE);
				return ;
			}
			if (fd >= 0)
				close(fd);

			// Apply redirection to the current command
			if (cur->type == REDIR_IN)
			{
				free(minishell->cmd[cmd_index].input_file_name);
				minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
				if (!minishell->cmd[cmd_index].input_file_name)
				{
					write(2, "minishell: memory allocation failed\n", 36);
					minishell->exit_status = 1;  // General error
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].in_type = REDIR_IN;
			}
			else if (cur->type == REDIR_OUT)
			{
				free(minishell->cmd[cmd_index].output_file_name);
				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
				if (!minishell->cmd[cmd_index].output_file_name)
				{
					write(2, "minishell: memory allocation failed\n", 36);
					minishell->exit_status = 1;  // General error
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].out_type = REDIR_OUT;
			}
			else if (cur->type == REDIR_APPEND)
			{
				free(minishell->cmd[cmd_index].output_file_name);
				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
				if (!minishell->cmd[cmd_index].output_file_name)
				{
					write(2, "minishell: memory allocation failed\n", 36);
					minishell->exit_status = 1;  // General error
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].out_type = REDIR_APPEND;
			}

			cur = cur->next; // skip the filename
		}
		else if (cur->type == HERE_DOC)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				write(2, "minishell: syntax error near redirection\n", 42);
				minishell->exit_status = 2;  // Incorrect usage
				exit(EXIT_FAILURE);
			}

			const char *filename = cur->next->value;

			// For multiple heredocs, only keep the last one
			// This ensures that only the final delimiter is used for the heredoc
			// The heredoc processing will read all content until the final delimiter
			free(minishell->cmd[cmd_index].input_file_name);
			minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
			if (!minishell->cmd[cmd_index].input_file_name)
			{
				write(2, "minishell: memory allocation failed\n", 36);
				minishell->exit_status = 1;  // General error
				exit(EXIT_FAILURE);
			}
			minishell->cmd[cmd_index].in_type = HERE_DOC;
			minishell->cmd[cmd_index].input_quote = cur->next->quote;

			cur = cur->next; // skip the filename
		}
		else if (cur->type == WORD)
		{
			minishell->cmd[cmd_index].argv[arg_index] = ft_strdup(cur->value);
			if (!minishell->cmd[cmd_index].argv[arg_index])
			{
				write(2, "minishell: memory allocation failed\n", 36);
				minishell->exit_status = 1;  // General error
				exit(EXIT_FAILURE);
			}
			arg_index++;
		}
		cur = cur->next;
	}

	// Null-terminate last command argv
	minishell->cmd[cmd_index].argv[arg_index] = NULL;
	
	// Handle heredoc without command (default to cat)
	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		if (minishell->cmd[i].in_type == HERE_DOC && 
			(!minishell->cmd[i].argv || !minishell->cmd[i].argv[0]))
		{
			// Add cat as default command only if there's no pipe after this command
			// or if this is the last command in the pipeline
			if (i == minishell->pipex_count)
			{
				minishell->cmd[i].argv[0] = ft_strdup("cat");
				if (!minishell->cmd[i].argv[0])
				{
					write(2, "minishell: memory allocation failed\n", 36);
					minishell->exit_status = 1;  // General error
					exit(EXIT_FAILURE);
				}
			}
			// If there's a pipe after this command, leave it as empty command
			// The heredoc content will flow to the next command
		}
	}
}
