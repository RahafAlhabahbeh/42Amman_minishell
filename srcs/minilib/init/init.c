
#include "../../../include/minishell.h"

void init(t_minishell *mini)
{
    mini->promp_input = NULL;
    mini->cmd = NULL;
    mini->token_count = 0;
    mini->pipex_count = 0;
    mini->token = NULL;
	mini->fd_out = -1;
	mini->env_list = NULL;
    ft_memset(mini->buff, 0, sizeof(mini->buff));
}