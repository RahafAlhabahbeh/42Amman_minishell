#include "../../../include/minishell.h"

void unset_env(t_env **env, const char *key)
{
    t_env *cur = *env;
    t_env *prev = NULL;

    while (cur)
    {
        if (ft_strncmp(cur->key, key, ft_strlen(key)) == 0)
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

void call_unset(t_minishell *mini, char **argv)
{
    for (int i = 1; argv[i]; i++)
    {
        if (!is_valid_identifier(argv[i]))
        {
            fprintf(stderr, "unset: `%s`: not a valid identifier\n", argv[i]);
            continue;
        }
        unset_env(&mini->env_list, argv[i]);
    }
}