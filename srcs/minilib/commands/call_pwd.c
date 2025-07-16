#include "minishell.h"

void call_pwd(t_minishell *minishell)
{
	printf("call pwd\n");
	int fd;
	
    	fd = minishell->fd_out;
    	if (fd == -1)
		fd = 1;
	if (getcwd(minishell->buff, sizeof(minishell->buff)) != NULL) 
	{
		write(fd, minishell->buff, ft_strlen(minishell->buff));
		write(fd, "\n", 1);
	}
	else
	{
                perror("getcwd() error");
        }
}