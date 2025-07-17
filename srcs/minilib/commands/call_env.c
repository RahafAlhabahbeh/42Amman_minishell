#include "../../../include/minishell.h"

void call_env(t_minishell *minishell, char **envp)
{
    printf("call env\n");
    int i = 0;
    int fd = (minishell->fd_out == -1) ? STDOUT_FILENO : minishell->fd_out;

    while (envp[i])
    {
        write(fd, envp[i], ft_strlen(envp[i]));
        write(fd, "\n", 1);
        i++;
    }
}
char	*my_getenv(char *name, char **env)
{
	int		i;
	size_t	len;

	if (!name || !env)
		return NULL;
		
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}
