/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:38 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/02 22:14:39 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_minishell *shell)
{
	shell->input = NULL;
	shell->i = 0;
	shell->j = 0;
	shell->fd_in = -1;
	shell->fd_out = -1;
	shell->fd_app = -1;
	shell->tok = NULL;
	shell->path = NULL;
	shell->user = NULL;
	shell->pwd = NULL;
	shell->pwd_old = NULL;
}

char	*my_getenv(char *name, char **env)
{
	int		i;
	int		j;
	char	*sub;

	i = 0;
	while (env[i])
	{
		j = 0;
		while (env[i][j] && env[i][j] != '=')
			j++;
		sub = ft_substr(env[i], 0, j);
		if (ft_strncmp(sub, name, ft_strlen(name)) == 0)
		{
			free(sub);
			return (env[i] + j + 1);
		}
		free(sub);
		i++;
	}
	return (NULL);
}


// user@<call_hostname>:<call_path>$
char *minishell_promp(t_minishell *shell)
{
	char *hostname = "minishell"; // or gethostname() if allowed
	int len = ft_strlen(shell->user) + ft_strlen(hostname)
			+ ft_strlen(shell->pwd) + 4; // '@' ':' '$' '\0'
	char *promp = malloc(len);
	if (!promp)
		return NULL;

	ft_strlcpy(promp, shell->user, len);
	ft_strlcat(promp, "@", len);
	ft_strlcat(promp, hostname, len);
	ft_strlcat(promp, ":", len);
	ft_strlcat(promp, shell->pwd, len);
	ft_strlcat(promp, "$", len);

	return promp;
}


void init_promp(t_minishell *shell, char **envp)
{
	//get_path(shell, envp);
	shell->path = my_getenv("PATH", envp);
	shell->user = my_getenv("USER", envp);
	shell->pwd = my_getenv("PWD", envp);
	//shell->pwd_old = getenv("OLDPWD"); // ??
	shell->pwd_old = my_getenv("OLDPWD", envp); // ??
	
	/*
	for(int i = 0; envp[i]; i++)
		printf("Env[%d]: %s\n", i, envp[i]);
	*/
	//printf("the path is: %s\n", shell->path);
	//printf("the user is: %s\n", shell->user);
	//printf("the pwd is: %s\n", shell->pwd);
	//printf("the old pwd is: %s\n", shell->pwd_old);
	//getpwd();
}
