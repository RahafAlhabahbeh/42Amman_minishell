#include "../../../include/minishell.h"

t_env *init_env_list(char **envp)
{
    t_env *head = NULL;
    t_env *tail = NULL;

    for (int i = 0; envp[i]; i++)
    {
        char *equal = ft_strchr(envp[i], '=');
        if (!equal)
            continue;

        size_t key_len = equal - envp[i];
        char *key = ft_substr(envp[i], 0, key_len);
        char *val = ft_strdup(equal + 1);

        t_env *node = malloc(sizeof(t_env));
        node->key = key;
        node->value = val;
        node->next = NULL;

        if (!head)
            head = node;
        else
            tail->next = node;
        tail = node;
    }

    return head;
}

char *get_env_value(const char *key, t_env *env)
{
    while (env)
    {
        if (ft_strncmp(env->key, key, ft_strlen(env->key) + 1) == 0)
            return env->value;
        env = env->next;
    }
    return NULL;
}

void set_env_value(t_env **env, const char *key, const char *value)
{
    t_env *cur = *env;

    while (cur)
    {
        if (ft_strncmp((*env)->key, key, ft_strlen((*env)->key)) == 0)
        {
            free(cur->value);
            cur->value = ft_strdup(value);
            return;
        }
        cur = cur->next;
    }

    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = *env;
    *env = new_node;
}

void unset_env(t_env **env, const char *key)
{
    t_env *cur = *env;
    t_env *prev = NULL;

    while (cur)
    {
        if (ft_strncmp((*env)->key, key, ft_strlen((*env)->key)) == 0
    && ft_strlen((*env)->key) == ft_strlen(key))
        {
            if (prev)
                prev->next = cur->next;
            else
                *env = cur->next;
            free(cur->key);
            free(cur->value);
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

