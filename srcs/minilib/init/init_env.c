#include "../../../include/minishell.h"

static void handle_shlvl(t_minishell *mini)
{
    char *shlvl_str = get_value_env(mini, "SHLVL");
    int shlvl = 0;
    
    if (shlvl_str)
    {
        shlvl = ft_atoi(shlvl_str);
        if (shlvl < 0)
            shlvl = 0;
    }
    
    shlvl++;
    if (shlvl > 999)
    {
        ft_putstr_fd("minishell: warning: shell level (", STDERR_FILENO);
        ft_putstr_fd(ft_itoa(shlvl), STDERR_FILENO);
        ft_putstr_fd(") too high, resetting to 1\n", STDERR_FILENO);
        shlvl = 1;
    }
    
    char *new_shlvl = ft_itoa(shlvl);
    if (new_shlvl)
    {
        set_env_value(mini, "SHLVL", new_shlvl);
        free(new_shlvl);
    }
}

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
        if (!key)
            continue;
            
        char *value = ft_strdup(eq + 1);
        if (!value)
        {
            free(key);
            continue;
        }

        mini->env_list = set_env_value(mini, key, value);

        free(key);
        free(value);
    }
    
    // Handle SHLVL
    handle_shlvl(mini);
}

char *get_env_value(const char *key, t_env *env)
{
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(env->key)) == 0)
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
    if (!new_node->key)
    {
        free(new_node);
        return mini->env_list;
    }
    
    if (value)
    {
        new_node->value = ft_strdup(value);
        if (!new_node->value)
        {
            free(new_node->key);
            free(new_node);
            return mini->env_list;
        }
    }
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
