
#include "../../../include/minishell.h"

void init(t_minishell *mini)
{
    mini->promp_input = NULL;
    mini->cmd = NULL;
    mini->cmd_count = 0;
    mini->token_count = 0;
    mini->pipex_count = 0;
    mini->token = NULL;
<<<<<<< HEAD
	mini->fd_out = -1;
	mini->env_list = NULL;
=======
    mini->fd_out = 1;  // Initialize to stdout
    mini->env_list = NULL;
    mini->exit_status = 0;
>>>>>>> main
    ft_memset(mini->buff, 0, sizeof(mini->buff));
}