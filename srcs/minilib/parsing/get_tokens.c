#include "../../../include/minishell.h"

static t_token *new_token(const char *value, t_token_type type, char quote)
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
    tok->quote = quote;
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

#include <ctype.h> // for isspace

t_token *tokenize(t_minishell *minishell)
{
    size_t i = 0;
    size_t len = strlen(minishell->promp_input);
    char buf[1024];
    int buf_i = 0;
    t_token *head = NULL, *tail = NULL;
    char current_quote = 0;
    char token_quote = 0;

    while (i < len)
    {
        char c = minishell->promp_input[i];

        if ((c == '\'' || c == '"') && (!current_quote || current_quote == c))
        {
            if (!current_quote)
            {
                current_quote = c;
                if (buf_i == 0)
                    token_quote = c;
            }
            else
                current_quote = 0;
            i++;
        }
        else if (!current_quote && (minishell->promp_input[i] == '>' || minishell->promp_input[i] == '<'))
        {
            if (buf_i > 0)
            {
                buf[buf_i] = '\0';
                append_token(&head, &tail, new_token(buf, WORD, token_quote));
                buf_i = 0;
                token_quote = 0;
            }

            // Handle << or >>
            if (minishell->promp_input[i] == '>' && minishell->promp_input[i + 1] == '>')
            {
                append_token(&head, &tail, new_token(">>", REDIR_APPEND, 0));
                i += 2;
            }
            else if (minishell->promp_input[i] == '<' && minishell->promp_input[i + 1] == '<')
            {
                append_token(&head, &tail, new_token("<<", HERE_DOC, 0));
                i += 2;
            }
            else if (minishell->promp_input[i] == '>')
            {
                append_token(&head, &tail, new_token(">", REDIR_OUT, 0));
                i++;
            }
            else if (minishell->promp_input[i] == '<')
            {
                append_token(&head, &tail, new_token("<", REDIR_IN, 0));
                i++;
            }
        }
        else if (c == '|' && !current_quote)
        {
            if (buf_i > 0)
            {
                buf[buf_i] = '\0';
                append_token(&head, &tail, new_token(buf, WORD, token_quote));
                buf_i = 0;
                token_quote = 0;
            }
            append_token(&head, &tail, new_token("|", PIPE, 0));
            i++;
        }
        else if (isspace((unsigned char)c) && !current_quote)
        {
            if (buf_i > 0)
            {
                buf[buf_i] = '\0';
                append_token(&head, &tail, new_token(buf, WORD, token_quote));
                buf_i = 0;
                token_quote = 0;
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
        append_token(&head, &tail, new_token(buf, WORD, token_quote));
    }

    return head;
}
