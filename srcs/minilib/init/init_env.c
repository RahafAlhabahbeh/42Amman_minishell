#include "../../../include/minishell.h"

void init_env_list(t_minishell *mini, char **envp)
{
    mini->env_list = NULL;
    for (int i = 0; envp[i]; i++)
    {
        char *eq = ft_strchr(envp[i], '=');
        if (!eq)
            continue;

        int key_len = eq - envp[i];
        char *key = ft_substr(envp[i], 0, key_len);
        char *value = ft_strdup(eq + 1);

        mini->env_list = set_env_value(mini, key, value);

        free(key);
        free(value);
    }
}

char *get_env_value(const char *key, t_env *env)
{
    while (env)
    {
        if (ft_strcmp(env->key, key) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

t_env *set_env_value(t_minishell *mini, char *key, char *value)
{
    t_env *cur = mini->env_list;

    // If key exists, update value
    while (cur)
    {
        if (ft_strcmp(cur->key, key) == 0)
        {
            free(cur->value);
            if (value)
                cur->value = ft_strdup(value);
            else
                cur->value = NULL;
            return mini->env_list;
        }
        cur = cur->next;
    }

    // If key not found, add new node at the end
    t_env *new_node = malloc(sizeof(t_env));
    if (!new_node)
        return mini->env_list; // handle malloc failure better if needed

    new_node->key = ft_strdup(key);
    if (value)
        new_node->value = ft_strdup(value);
    else
        new_node->value = NULL;
    new_node->next = NULL;

    if (!mini->env_list)
    {
        mini->env_list = new_node;
    }
    else
    {
        t_env *last = mini->env_list;
        while (last->next)
            last = last->next;
        last->next = new_node;
    }

    return mini->env_list;
}
