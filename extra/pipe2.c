#include "../libft/libft.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void ft_free_split(char **split)
{
	int i = 0;
	if (!split)
		return;
	while (split[i])
		free(split[i++]);
	free(split);
}

char *build_path(char *dir, const char *cmd)
{
	char *temp;
	char *full;

	if (dir[ft_strlen(dir) - 1] != '/')
		temp = ft_strjoin(dir, "/");
	else
		temp = ft_strdup(dir);
	if (!temp)
		return NULL;

	full = ft_strjoin(temp, cmd);
	free(temp);
	return full;
}

char *search_in_path(char **token, const char *cmd)
{
	char *full;
	int i = 0;

	while (token[i])
	{
		full = build_path(token[i], cmd);
		if (!full)
			break;
		if (access(full, X_OK) == 0)
			return full;
		free(full);
		i++;
	}
	return NULL;
}

char *find_executable_path(const char *cmd, char **envp)
{
	char *path_env = NULL;
	char *path_copy;
	char *result;
	char **token;

	if (!cmd || !*cmd)
		return NULL;
	if (strchr(cmd, '/'))
		return (access(cmd, X_OK) == 0) ? ft_strdup(cmd) : NULL;

	for (int i = 0; envp[i]; i++)
		if (!ft_strncmp(envp[i], "PATH=", 5))
			path_env = envp[i] + 5;
	if (!path_env)
		return NULL;

	path_copy = ft_strdup(path_env);
	if (!path_copy)
		return NULL;
	token = ft_split(path_copy, ':');
	free(path_copy);

	result = search_in_path(token, cmd);
	ft_free_split(token);
	return result;
}

int main(int ac, char **av, char **envp)
{
	if (ac < 2)
	{
		fprintf(stderr, "Usage: %s <command>\n", av[0]);
		return 1;
	}
	char *full_path = find_executable_path(av[1], envp);
	if (full_path)
	{
		printf("Found: %s\n", full_path);
		free(full_path);
	}
	else
		printf("Command not found in PATH.\n");
	return 0;
}

