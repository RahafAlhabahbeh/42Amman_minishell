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

	init_cmd(minishell);

	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		minishell->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
		if (!minishell->cmd[i].argv)
		{
			perror("malloc");
			exit(EXIT_FAILURE);
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
				fprintf(stderr, "minishell: internal error: too many commands\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
				 cur->type == REDIR_APPEND || cur->type == HERE_DOC)
		{
			if (!cur->next || cur->next->type != WORD)
			{
				fprintf(stderr, "minishell: syntax error near redirection\n");
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
			// HERE_DOC is handled separately, skip open()

			if ((cur->type != HERE_DOC) && fd < 0)
			{
				perror(filename);
				exit(EXIT_FAILURE);
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
					fprintf(stderr, "minishell: memory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].in_type = REDIR_IN;
			}
			else if (cur->type == HERE_DOC)
			{
				// For multiple heredocs, only keep the last one
				free(minishell->cmd[cmd_index].input_file_name);
				minishell->cmd[cmd_index].input_file_name = ft_strdup(filename);
				if (!minishell->cmd[cmd_index].input_file_name)
				{
					fprintf(stderr, "minishell: memory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].in_type = HERE_DOC;
				minishell->cmd[cmd_index].input_quote = cur->next->quote;
			}
			else if (cur->type == REDIR_OUT)
			{
				free(minishell->cmd[cmd_index].output_file_name);
				minishell->cmd[cmd_index].output_file_name = ft_strdup(filename);
				if (!minishell->cmd[cmd_index].output_file_name)
				{
					fprintf(stderr, "minishell: memory allocation failed\n");
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
					fprintf(stderr, "minishell: memory allocation failed\n");
					exit(EXIT_FAILURE);
				}
				minishell->cmd[cmd_index].out_type = REDIR_APPEND;
			}

			cur = cur->next; // skip the filename
		}
		else if (cur->type == WORD)
		{
			minishell->cmd[cmd_index].argv[arg_index] = ft_strdup(cur->value);
			if (!minishell->cmd[cmd_index].argv[arg_index])
			{
				fprintf(stderr, "minishell: memory allocation failed\n");
				exit(EXIT_FAILURE);
			}
			arg_index++;
		}
		cur = cur->next;
	}

	// Null-terminate last command argv
	minishell->cmd[cmd_index].argv[arg_index] = NULL;
}
