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

char *replace_var(const char *str, char **envp, char quote)
{
    if (quote == '\'') // no expansion in single quotes
        return strdup(str);

    char result[1024] = {0};
    int i = 0, j = 0;

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++;
            // Skip '$?' for now
            if (str[i] == '?')
            {
                // Handle $? if you want (not covered here)
                i++;
                continue;
            }

            char var[256] = {0};
            int k = 0;

            // Extract variable name
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                var[k++] = str[i++];
            var[k] = '\0';

            const char *val = my_getenv(var, envp);
            if (!val)
                val = "";

            size_t len = strlen(val);
            if (j + len < sizeof(result))
            {
                strcat(result, val);
                j += len;
            }
        }
        else
        {
            if (j < (int)(sizeof(result) - 1))
                result[j++] = str[i++];
        }
    }

    result[j] = '\0';
    return strdup(result);
}



t_token *expand(t_minishell *minishell, char **envp)
{
    t_token *cur = minishell->token;
    t_token *new_list = NULL;
    t_token *tail = NULL;

    while (cur)
    {
        char *expanded = replace_var(cur->value, envp, cur->quote);
        t_token *new_tok = malloc(sizeof(t_token));
        if (!new_tok)
            return NULL;

        new_tok->value = expanded;
        new_tok->type = cur->type;
        new_tok->quote = cur->quote;
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

