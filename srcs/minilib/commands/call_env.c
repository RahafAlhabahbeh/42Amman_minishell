/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/24 08:54:24 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

void call_env(t_minishell *mini)
{
    print_env_list(mini->env_list);
}

char *get_value_env(t_minishell *mini, const char *key)
{
    t_env *cur = mini->env_list;

    while (cur)
    {
        if (ft_strncmp(cur->key, key, ft_strlen(cur->key)) == 0)
            return cur->value;  // return the value pointer (do not free it)
        cur = cur->next;
    }
    return NULL; // not found
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

