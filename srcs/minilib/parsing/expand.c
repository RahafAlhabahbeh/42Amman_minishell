#include "../../../include/minishell.h"

char *replace_var(t_minishell *minishell, const char *str, char quote)
{
    char result[1024] = {0};
    int i = 0, j = 0;

    if (quote == '\'')
    {
        char *dup = strdup(str);
        if (!dup)
            return NULL;
        return dup;
    }

    while (str[i])
    {
        if (str[i] == '$' && str[i + 1])
        {
            i++;
            if (str[i] == '?')
            {
                i++;
                char status_str[12]; // Enough for any int
                snprintf(status_str, sizeof(status_str), "%d", minishell->exit_status);

                if (j + (int)strlen(status_str) < (int)sizeof(result))
                {
                    strcat(result, status_str);
                    j += strlen(status_str);
                }
                continue;
            }

            char var[256] = {0};
            int k = 0;

            // Extract variable name
            while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
                var[k++] = str[i++];
            var[k] = '\0';

            const char *val = get_value_env(minishell, var);
            if (!val)
                val = "";

            size_t len = strlen(val);
            if (j + (int)len < (int)sizeof(result))
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

    char *dup = strdup(result);
    if (!dup)
        return NULL;
    return dup;
}

t_token *expand(t_minishell *minishell)
{
    t_token *cur = minishell->token;
    t_token *new_list = NULL;
    t_token *tail = NULL;

    while (cur)
    {
        char *expanded = replace_var(minishell, cur->value, cur->quote);
        if (!expanded)
        {
            // On failure, free already created tokens and return NULL
            while (new_list)
            {
                t_token *tmp = new_list;
                new_list = new_list->next;
                free(tmp->value);
                free(tmp);
            }
            return NULL;
        }

        t_token *new_tok = malloc(sizeof(t_token));
        if (!new_tok)
        {
            free(expanded);
            // Free all previously allocated tokens
            while (new_list)
            {
                t_token *tmp = new_list;
                new_list = new_list->next;
                free(tmp->value);
                free(tmp);
            }
            return NULL;
        }

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
