#include "../libft/libft.h"
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void ft_free_split(char **split)
{
	int i = 0;
	if (!split)
		return;
	while (split[i])
		free(split[i++]);
	free(split);
}

char *find_executable_path(const char *cmd, char **envp) {
    if (!cmd || !*cmd)
        return NULL;

    if (strchr(cmd, '/')) {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;
    }

    char *path_env = NULL;
    for (int i = 0; envp[i]; ++i) {
        if (ft_strncmp(envp[i], "PATH=", 5) == 0) {
            path_env = envp[i] + 5;
            break;
        }
    }

    if (!path_env)
        return NULL;

    char *path_copy = ft_strdup(path_env);
    if (!path_copy)
        return NULL;

    char **token = ft_split(path_copy, ':');
    free(path_copy);

    int i = 0;
    while (token[i]) {
        char *temp;
        if (token[i][ft_strlen(token[i]) - 1] != '/')
            temp = ft_strjoin(token[i], "/");
        else
            temp = ft_strdup(token[i]);

        if (!temp) {
            ft_free_split(token);
            return NULL;
        }

        char *full_path = ft_strjoin(temp, cmd);
        free(temp);

        if (!full_path) {
            ft_free_split(token);
            return NULL;
        }

        if (access(full_path, X_OK) == 0) {
            ft_free_split(token);
            return full_path;
        }

        free(full_path);
        i++;
    }

    ft_free_split(token);
    return NULL;
}

int main(int ac, char **av, char **envp) {
    if (ac < 2) {
        fprintf(stderr, "Usage: %s <command>\n", av[0]);
        return 1;
    }

    char *full_path = find_executable_path(av[1], envp);
    if (full_path) {
        printf("Found: %s\n", full_path);
        free(full_path);
    } else {
        printf("Command not found in PATH.\n");
    }

    return 0;
}

