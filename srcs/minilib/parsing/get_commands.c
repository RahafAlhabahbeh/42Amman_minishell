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

#define MAX_ARGS 512 // Adjust as needed

void put_token_to_commands(t_minishell *minishell)
{
	t_token *cur = minishell->token;
	int cmd_index = 0;
	int arg_index = 0;

	// Allocate argv for each command
	for (int i = 0; i <= minishell->pipex_count; i++)
	{
		minishell->cmd[i].argv = malloc(sizeof(char *) * MAX_ARGS);
		if (!minishell->cmd[i].argv)
			exit(EXIT_FAILURE);
		minishell->cmd[i].input_file_name = NULL;
		minishell->cmd[i].output_file_name = NULL;
	}

	while (cur)
	{
		if (cur->type == PIPE)
		{
			// End current command
			minishell->cmd[cmd_index].argv[arg_index] = NULL;
			cmd_index++;
			arg_index = 0;
		}
		else if (cur->type == REDIR_IN || cur->type == REDIR_OUT ||
				 cur->type == REDIR_APPEND || cur->type == HERE_DOC)
		{

			// Make sure the next token exists and is a WORD
			if (!cur->next || cur->next->type != WORD)
			{
				fprintf(stderr, "Syntax error: expected filename after redirection\n");
				exit(EXIT_FAILURE);
			}

			if (cur->type == REDIR_IN)
			{
				minishell->cmd[cmd_index].input_file_name = strdup(cur->next->value);
			}
			else
			{
				// Handle REDIR_OUT and REDIR_APPEND (same behavior for now)
				minishell->cmd[cmd_index].output_file_name = strdup(cur->next->value);
			}

			cur = cur->next; // skip the filename token
		}
		else if (cur->type == WORD)
		{
			minishell->cmd[cmd_index].argv[arg_index++] = strdup(cur->value);
		}

		cur = cur->next;
	}

	// Terminate the last argv array
	minishell->cmd[cmd_index].argv[arg_index] = NULL;
}
