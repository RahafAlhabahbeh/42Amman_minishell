
#include "../../../include/minishell.h"

void init(t_minishell *mini)
{
    mini->promp_input = NULL;
    mini->cmd = NULL;
    mini->token_count = 0;
    mini->pipex_count = 0;
    mini->token = NULL;
	mini->fd_out = -1;
    ft_memset(mini->buff, 0, sizeof(mini->buff));
}

// t_cmd *init_cmd(void)
// {
//     t_cmd *cmd = malloc(sizeof(t_cmd));
//     if (!cmd)
//         return NULL;

//     cmd->argv = NULL;
//     cmd->in_type = WORD;          // You can use a separate enum like NONE if needed
//     cmd->out_type = WORD;
//     cmd->input_file_name = NULL;
//     cmd->output_file_name = NULL;
//     cmd->next = NULL;
//     cmd->prev = NULL;
//     return cmd;
// }

// t_env *init_env(const char *key, const char *value, int index)
// {
//     t_env *env = malloc(sizeof(t_env));
//     if (!env)
//         return NULL;

//     env->index = index;
//     env->key = key ? strdup(key) : NULL;
//     env->value = value ? strdup(value) : NULL;
//     env->next = NULL;

//     return env;
// }

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

 
// t_env *init_env_list(char **envp)
// {
// 	t_env *head = NULL;
// 	t_env *tail = NULL;
// 	int index = 0;

// 	for (int i = 0; envp[i]; i++, index++)
// 	{
// 		char *equal = ft_strchr(envp[i], '=');
// 		if (!equal)
// 			continue;

// 		size_t key_len = equal - envp[i];
// 		char *key = ft_substr(envp[i], 0, key_len);
// 		char *val = ft_strdup(equal + 1);
// 		t_env *node = init_env(key, val, index);

// 		free(key);
// 		free(val);

// 		if (!node)
// 			continue;

// 		if (!head)
// 			head = node;
// 		else
// 			tail->next = node;
// 		tail = node;
// 	}
// 	return head;
// }

// void free_env_list(t_env *env)
// {
// 	t_env *tmp;
// 	while (env)
// 	{
// 		tmp = env->next;
// 		free(env->key);
// 		free(env->value);
// 		free(env);
// 		env = tmp;
// 	}
// } 

// // user@<call_hostname>:<call_path>$
// char *minishell_promp(t_minishell *shell)
// {
// 	char *hostname = "minishell";
// 	int len = ft_strlen(shell->user) + ft_strlen(hostname)
// 			+ ft_strlen(shell->pwd) + 4; // '@' ':' '$' '\0'
// 	char *promp = malloc(len);
// 	if (!promp)
// 		return NULL;

// 	ft_strlcpy(promp, shell->user, len);
// 	ft_strlcat(promp, "@", len);
// 	ft_strlcat(promp, hostname, len);
// 	ft_strlcat(promp, ":", len);
// 	ft_strlcat(promp, shell->pwd, len);
// 	ft_strlcat(promp, "$", len);

// 	return promp;
// }


// void init_promp(t_minishell *shell, char **envp)
// {
// 	//get_path(shell, envp);
// 	shell->path = my_getenv("PATH", envp);
// 	shell->user = my_getenv("USER", envp);
// 	shell->pwd = my_getenv("PWD", envp);
// 	//shell->pwd_old = getenv("OLDPWD"); // ??
// 	shell->pwd_old = my_getenv("OLDPWD", envp); // ??
	
// 	/*
// 	for(int i = 0; envp[i]; i++)
// 		printf("Env[%d]: %s\n", i, envp[i]);
// 	*/
// 	//printf("the path is: %s\n", shell->path);
// 	//printf("the user is: %s\n", shell->user);
// 	//printf("the pwd is: %s\n", shell->pwd);
// 	//printf("the old pwd is: %s\n", shell->pwd_old);
// 	//getpwd();
// }