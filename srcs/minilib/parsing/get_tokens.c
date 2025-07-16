#include "../../../include/minishell.h"

void free_tokens(t_token *head)
{
    t_token *tmp;
    while (head)
    {
        tmp = head->next;
        free(head->value);
        free(head);
        head = tmp;
    }
}
static t_token *new_token(const char *value, t_token_type type)
{
    t_token *tok = malloc(sizeof(t_token));
    if (!tok)
        return (NULL);
    tok->value = strdup(value);
    if (!tok->value)
    {
        free(tok);
        return (NULL);
    }
    tok->type = type;
    tok->next = NULL;
    return (tok);
}

static void append_token(t_token **head, t_token **tail, t_token *tok)
{
    if (!*head)
        *head = *tail = tok;
    else
    {
        (*tail)->next = tok;
        *tail = tok;
    }
}

// removes quotes like: "'ls'" => ls, """ls""" => ls, ''ls'' => ls
static char *strip_quotes(const char *str)
{
    int i = 0, j = 0;
    char quote = 0;
    size_t len = strlen(str);
    char *result = malloc(len + 1);

    if (!result)
        return NULL;

    while (str[i])
    {
        if ((str[i] == '\'' || str[i] == '"') && (!quote || quote == str[i]))
        {
            if (!quote)
                quote = str[i]; // opening quote
            else
                quote = 0; // closing quote
        }
        else
            result[j++] = str[i];
        i++;
    }
    result[j] = '\0';
    return result;
}

t_token *tokenize(t_minishell *minishell)
{
    size_t i = 0;
    size_t len = strlen(minishell->promp_input);
    char buf[1024];
    int buf_i = 0;
    t_token *head = NULL, *tail = NULL;
    char quote = 0;

    while (i < len)
    {
        char c = minishell->promp_input[i];

        if ((c == '\'' || c == '"') && (!quote || quote == c))
        {
            if (!quote)
                quote = c; // opening quote
            else
                quote = 0; // closing quote
            buf[buf_i++] = c;
            i++;
        }
        else if (isspace((unsigned char)c) && !quote)
        {
            if (buf_i > 0)
            {
                buf[buf_i] = '\0';
                char *clean = strip_quotes(buf);
                if (clean)
                    append_token(&head, &tail, new_token(clean, WORD));
                free(clean);
                buf_i = 0;
            }
            i++;
        }
        else
        {
            buf[buf_i++] = c;
            i++;
        }
    }

    if (buf_i > 0)
    {
        buf[buf_i] = '\0';
        char *clean = strip_quotes(buf);
        if (clean)
            append_token(&head, &tail, new_token(clean, WORD));
        free(clean);
    }

    return head;
}