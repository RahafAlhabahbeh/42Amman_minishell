#include "../../../include/minishell.h"

// #include "minishell.h"

// char *get_env_value(const char *var, t_minishell *minishell)
// {
//     // t_env *env = minishell->env_list;
//     while (env)
//     {
//         if (ft_strncmp(env->key, var, ft_strlen(var)) == 0)
//             return env->value;
//         env = env->next;
//     }
//     return "";
// }

char *replace_var(const char *str, char **envp)
{
    char result[1024] = {0};  // Final expanded string
    int i = 0, j = 0;

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1]) || str[i + 1] == '_'))
        {
            i++; // skip '$'
            char var[256] = {0};
            int k = 0;

            // Extract variable name
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                var[k++] = str[i++];
            var[k] = '\0';

            const char *val = my_getenv(var, envp);
            // printf("VAR: %s\n", var);
            // printf("ENV: %s\n", minishell->envp[0]);
            // printf("VALUE: %s\n", val);
            if (val)
            {
                size_t len = ft_strlen(val);
                if (j + len < sizeof(result))
                {
                    ft_strlcat(result, val, sizeof(result));
                    j += len; // <-- THIS was missing
                }
            }
        }
        else
        {
            // Copy character if there's space
            if (j < (int)(sizeof(result) - 1))
                result[j++] = str[i++];
        }
    }

    result[j] = '\0';
    return ft_strdup(result);
}



t_token *expand(t_minishell *minishell, char **envp)
{
    t_token *cur = minishell->token;
    t_token *new_list = NULL;
    t_token *tail = NULL;

    while (cur)
    {
        char *expanded = replace_var(cur->value, envp);
        t_token *new_tok = malloc(sizeof(t_token));
        new_tok->value = expanded;
        new_tok->type = cur->type;
        new_tok->next = NULL;

        if (!new_list)
            new_list = new_tok;
        else
            tail->next = new_tok;

        tail = new_tok;
        cur = cur->next;
    }

    return new_list;
}
