/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/04 09:29:16 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// Simple strcat implementation  
static char *simple_strcat(char *dest, const char *src)
{
    char *ptr = dest;
    while (*ptr)
        ptr++;
    while (*src)
        *ptr++ = *src++;
    *ptr = '\0';
    return dest;
}

void print_env_list(t_env *env_list)
{
    t_env *cur = env_list;
    while (cur)
    {
        if (cur->value != NULL)
            printf("%s=%s\n", cur->key, cur->value);
        cur = cur->next;
    }
}

void call_env(t_minishell *shell __attribute__((unused)), char **argv)
{
    if (argv[1])
    {
        ft_putstr_fd("env: too many arguments\n", STDERR_FILENO);
        shell->exit_status = 1;
        return;
    }

    t_env *curr = shell->env_list;
    while (curr)
    {
        if (curr->value)
        {
            ft_putstr_fd(curr->key, STDOUT_FILENO);
            ft_putchar_fd('=', STDOUT_FILENO);
            ft_putstr_fd(curr->value, STDOUT_FILENO);
            ft_putchar_fd('\n', STDOUT_FILENO);
        }
        curr = curr->next;
    }
}

char *get_value_env(t_minishell *mini, const char *key)
{
    t_env *cur = mini->env_list;

    while (cur)
    {
        if (ft_strcmp(cur->key, key) == 0)
            return cur->value;  // return the value pointer (do not free it)
        cur = cur->next;
    }
    return NULL; // not found
}

char *join_path(const char *dir, const char *cmd)
{
    if (!dir || !cmd)
        return NULL;
        
    char *full = malloc(ft_strlen(dir) + ft_strlen(cmd) + 2); // '/' + '\0'
    if (!full)
        return NULL;
    
    // Use simple string operations instead of snprintf
    full[0] = '\0';
    simple_strcat(full, dir);
    simple_strcat(full, "/");
    simple_strcat(full, cmd);
    return full;
}

char *resolve_cmd_path(char *cmd, char **envp)
{
    if (!cmd)
        return NULL;

    // If command contains a slash, try to exec directly
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return ft_strdup(cmd);
        else
            return NULL;
    }

    // Get PATH from envp
    char *path_var = NULL;
    for (int i = 0; envp[i]; i++)
    {
        // Check if this environment variable starts with "PATH="
        if (envp[i][0] == 'P' && envp[i][1] == 'A' && envp[i][2] == 'T' && 
            envp[i][3] == 'H' && envp[i][4] == '=')
        {
            path_var = envp[i] + 5;
            break;
        }
    }

    if (!path_var)
        return NULL;

    // Split PATH and check each directory for the command
    char **path_dirs = ft_split(path_var, ':');
    if (!path_dirs)
        return NULL;
        
    int i = 0;
    while (path_dirs[i])
    {
        char *full = join_path(path_dirs[i], cmd);
        if (full && access(full, X_OK) == 0)
        {
            // Free the split array before returning
            int j = 0;
            while (path_dirs[j])
                free(path_dirs[j++]);
            free(path_dirs);
            return full; // found executable path
        }
        free(full);
        i++;
    }

    // Free the split array
    int j = 0;
    while (path_dirs[j])
        free(path_dirs[j++]);
    free(path_dirs);
    return NULL;
}
