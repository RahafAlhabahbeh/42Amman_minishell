/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/08 08:39:38 by dal-mahr         ###   ########.fr       */
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
int is_executable(const char *path)
{
    struct stat sb;

    if (access(path, F_OK) != 0)
        return -1; // Not found
    if (access(path, X_OK) != 0)
        return -2; // Found but not executable

    if (stat(path, &sb) == 0 && S_ISREG(sb.st_mode))
        return 0; // Found and executable
    return -2;
}

// Main function: resolve cmd path
char *resolve_cmd_path(char *cmd, char **envp)
{
    if (!cmd || !*cmd)
        return NULL;

    // Case 1: Absolute or relative path
    if (cmd[0] == '/' || (cmd[0] == '.' && (cmd[1] == '/' || cmd[1] == '.')))
    {
        int status = is_executable(cmd);
        if (status == 0)
            return strdup(cmd);
        else if (status == -2)
        {
            fprintf(stderr, "%s: Permission denied\n", cmd);
            exit(126);
        }
        return NULL; // Not found
    }

    // Case 2: Search in PATH
    char *path_env = NULL;
    for (int i = 0; envp[i]; i++)
    {
        if (strncmp(envp[i], "PATH=", 5) == 0)
        {
            path_env = envp[i] + 5;
            break;
        }
    }
    if (!path_env)
        return NULL;

    char **paths = ft_split(path_env, ':');
    for (int i = 0; paths[i]; i++)
    {
        char *full = malloc(strlen(paths[i]) + strlen(cmd) + 2);
        if (!full) continue;
        sprintf(full, "%s/%s", paths[i], cmd);

        int status = is_executable(full);
        if (status == 0)
        {
            free(paths);
            return full;
        }
        else if (status == -2)
        {
            fprintf(stderr, "%s: Permission denied\n", cmd);
            free(paths);
            free(full);
            exit(126);
        }
        free(full);
    }
    free(paths);
    return NULL; // Not found anywhere
}