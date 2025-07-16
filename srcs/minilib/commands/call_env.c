#include "../../../include/minishell.h"

void call_env(t_minishell *minishell, char **envp)
{
    printf("call env\n");
    int i;
    i = 0;
    int fd;
    fd = minishell->fd_out;
    if (fd == -1)
        fd = 1;
    while(envp[i])
    {
    	write(fd, envp[i], ft_strlen(envp[i]));
    	write(fd, "\n", 1);
    	i++;
    }
}