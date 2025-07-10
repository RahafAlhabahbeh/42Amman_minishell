#include "minishell.h"

void redir_compare2(t_minishell *minishell)
{
	if (!strcmp(minishell->tok[minishell->i], ">>"))
	{
		minishell->fd_app = open(minishell->tok[minishell->i + 1], O_RDWR | O_CREAT | O_APPEND, 0777);
		if (minishell->fd_app == -1)
			perror("ERROR\n");
		minishell->i += 2;
	}
	else
	{
		minishell->cmd[minishell->j] = strdup(minishell->tok[minishell->i++]);
		if (!minishell->cmd[minishell->j])
			exit(1);
		minishell->j++;
	}
}