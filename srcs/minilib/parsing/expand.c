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

// Simple function to convert int to string (replaces snprintf)
static void int_to_str(int num, char *str)
{
    int i = 0;
    int is_negative = 0;
    
    if (num == 0)
    {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }
    
    while (num > 0)
    {
        str[i++] = (num % 10) + '0';
        num /= 10;
    }
    
    if (is_negative)
        str[i++] = '-';
    
    str[i] = '\0';
    
    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end)
    {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char *replace_var(t_minishell *minishell, const char *str, char quote)
{
    char result[1024] = {0};
    int i = 0, j = 0;

    if (quote == '\'')
    {
        char *dup = ft_strdup(str);
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
                int_to_str(minishell->exit_status, status_str);

                if (j + (int)ft_strlen(status_str) < (int)sizeof(result))
                {
                    simple_strcat(result, status_str);
                    j += ft_strlen(status_str);
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

            size_t len = ft_strlen(val);
            if (j + (int)len < (int)sizeof(result))
            {
                simple_strcat(result, val);
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

    char *dup = ft_strdup(result);
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
