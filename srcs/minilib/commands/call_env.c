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

char *join_path(const char *dir, const char *cmd)
{
    char *full = malloc(strlen(dir) + strlen(cmd) + 2); // '/' + '\0'
    if (!full) return NULL;
    sprintf(full, "%s/%s", dir, cmd);
    return full;
}

char *resolve_cmd_path(char *cmd, char **envp)
{
    if (!cmd)
        return NULL;

    // If command contains a slash, try to exec directly
    if (strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return strdup(cmd);
        else
            return NULL;
    }

    // Get PATH
    char *path_var = NULL;
    for (int i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_var = envp[i] + 5;
            break;
        }
    }

    if (!path_var)
        return NULL;

    // Split PATH and check each dir
    char *paths = strdup(path_var);
    char *token = strtok(paths, ":");

    while (token)
    {
        char *full = join_path(token, cmd);
        if (full && access(full, X_OK) == 0)
        {
            free(paths);
            return full; // success
        }
        free(full);
        token = strtok(NULL, ":");
    }

    free(paths);
    return NULL;
}

