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


void put_token_to_commands(t_minishell *minishell)
{
	t_token *cur = minishell->token;

	for (int cmd_i = 0; cmd_i <= minishell->pipex_count; cmd_i++)
	{
		int argc = count_args_for_command(cur);  // count WORD tokens
		minishell->cmd[cmd_i].argv = calloc(argc + 1, sizeof(char *)); // ✅ NULL-terminated

		int arg_index = 0;
		while (cur && cur->type != PIPE)
		{
			if (cur->type == WORD)
				minishell->cmd[cmd_i].argv[arg_index++] = strdup(cur->value);

			// You can add redirection handling here

			cur = cur->next;
		}
		if (cur && cur->type == PIPE)
			cur = cur->next;
	}
}


