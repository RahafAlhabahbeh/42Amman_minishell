#include "minishell.h"
// char *getenv(const char *name);
void call_env(t_minishell *minishell, char **envp)
{
    printf("call env\n");
    int i;
    i = 0;
    while(envp[i])
    {
    	write(minishell->fd_out, envp[i], ft_strlen(envp[i]));
    	write(minishell->fd_out, "\n", 1);
    	i++;
    }
}
/*
void call_cd(t_minishell *minishell, char **envp)
{
	printf("call cd with relative or absolute path only\n");
	printf("Path you put %s\n", minishell->cmd[1]);
	printf("current pwd function call my_getenv %s\n", my_getenv("PWD", envp));
	printf("current pwd from minishell %s\n", minishell->pwd);
	if (minishell->fd_out < 0)
		minishell->fd_out = STDOUT_FILENO;
	char *old_pwd = getcwd(NULL, 0); // safely get current directory
	char *new_pwd;

	if (chdir(minishell->cmd[1]) == -1)
	{
		perror("cd");
		free(old_pwd);
		return;
	}

	new_pwd = getcwd(NULL, 0);
	envp = call_export(envp, "OLDPWD", old_pwd);
	envp = call_export(envp, "PWD", new_pwd);

	minishell->pwd = ft_strdup(new_pwd);

	free(old_pwd);
	free(new_pwd);
	printf("updated pwd function call my_getenv %s\n", my_getenv("PWD", envp));
	printf("updated pwd from minishell %s\n", minishell->pwd);
	minishell->fd_out = -1;
}
*/
void call_cd(t_minishell *minishell, char **envp)
{
    printf("call cd with relative or absolute path only\n");
    printf("Path you put %s\n", minishell->cmd[1]);
    printf("current pwd function call my_getenv %s\n", my_getenv("PWD", envp));
    printf("current pwd from minishell %s\n", minishell->pwd);

    if (minishell->fd_out < 0)
        minishell->fd_out = STDOUT_FILENO;

    char *old_pwd = getcwd(NULL, 0); // Get current directory
    char *new_pwd;

    if (chdir(minishell->cmd[1]) == -1)
    {
        perror("cd");
        free(old_pwd);
        return;
    }

    new_pwd = getcwd(NULL, 0); // Get new directory after `chdir`

    // Create "OLDPWD=value" string and export
    char *oldpwd_var = malloc(strlen("OLDPWD=") + strlen(old_pwd) + 1);
    sprintf(oldpwd_var, "OLDPWD=%s", old_pwd);
    envp = call_export(envp, oldpwd_var);
    free(oldpwd_var);

    // Create "PWD=value" string and export
    char *pwd_var = malloc(strlen("PWD=") + strlen(new_pwd) + 1);
    sprintf(pwd_var, "PWD=%s", new_pwd);
    envp = call_export(envp, pwd_var);
    free(pwd_var);

    // Update internal minishell pwd
    minishell->pwd = ft_strdup(new_pwd);

    free(old_pwd);
    free(new_pwd);

    printf("updated pwd function call my_getenv %s\n", my_getenv("PWD", envp));
    printf("updated pwd from minishell %s\n", minishell->pwd);

    minishell->fd_out = -1;
}



char **call_export(char **envp, char *new_env_var)
{
    // new_env_var is "NAME=VALUE"
    int i = 0;
    int name_len;
    char *name;
    char *value;
    char *new_var;
    char **new_env;

    // Split new_env_var into name and value parts
    // Find '=' in new_env_var
    char *eq_pos = strchr(new_env_var, '=');
    if (!eq_pos)
        return envp; // invalid input, no '=' found

    name_len = eq_pos - new_env_var;
    name = new_env_var;
    value = eq_pos + 1;

    // Allocate memory for "NAME=VALUE"
    new_var = malloc(name_len + 1 + strlen(value) + 1);
    if (!new_var)
        return envp;

    // Copy "NAME=VALUE" manually
    memcpy(new_var, name, name_len);
    new_var[name_len] = '=';
    strcpy(new_var + name_len + 1, value);

    // Check if variable already exists in envp
    while (envp[i])
    {
        if (strncmp(envp[i], name, name_len) == 0 && envp[i][name_len] == '=')
        {
            //free(envp[i]);      // free old variable string
            envp[i] = new_var;  // replace with new variable
            return envp;
        }
        i++;
    }

    // Variable does not exist, create new env array with one extra slot
    new_env = malloc(sizeof(char *) * (i + 2)); // +1 for new var, +1 for NULL
    if (!new_env)
    {
        free(new_var);
        return envp;
    }

    // Copy old env pointers
    for (int j = 0; j < i; j++)
        new_env[j] = envp[j];

    new_env[i] = new_var;
    new_env[i + 1] = NULL;

    //free(envp); // free old env array pointer, but not strings (they are copied)
    return new_env;
}

char **call_unset(char **envp, char *del_name)
{
    int i = 0, j = 0;
    int len = strlen(del_name);
    int count = 0;

    // Count how many env vars we have
    while (envp[count])
        count++;

    // Allocate new environment array (max size is count, because one var will be removed)
    char **new_env = malloc(sizeof(char *) * count);
    if (!new_env)
        return envp;

    while (envp[i])
    {
        // If envp[i] does NOT start with del_name + '=' then keep it
        if (!(strncmp(envp[i], del_name, len) == 0 && envp[i][len] == '='))
        {
            new_env[j++] = envp[i];
        }
        else
        {
            //free(envp[i]); // free string to be deleted
        }
        i++;
    }
    new_env[j] = NULL;

    //free(envp); // free old env array pointer, not strings as they are moved or freed

    return new_env;
}