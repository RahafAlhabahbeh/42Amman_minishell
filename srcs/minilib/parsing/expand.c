#include "../../../include/minishell.h"
#include <pwd.h>
#include <unistd.h>

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

// Enhanced function to extract variable name with support for braced syntax
static int extract_var_name(const char *str, int *pos, char *var_name, int *is_braced)
{
    int i = *pos;
    int k = 0;
    *is_braced = 0;

    // Check for braced syntax ${VAR}
    if (str[i] == '{')
    {
        *is_braced = 1;
        i++; // skip '{'

        while (str[i] && str[i] != '}' && k < 255)
        {
            if (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '?' || str[i] == '$')
                var_name[k++] = str[i];
            else
                break; // Invalid character in variable name
            i++;
        }

        if (str[i] == '}')
            i++; // skip '}'
        else
            return -1; // Unclosed brace
    }
    else
    {
        // Regular syntax $VAR
        while (str[i] && (ft_isalnum(str[i]) || str[i] == '_') && k < 255)
            var_name[k++] = str[i++];
    }

    var_name[k] = '\0';
    *pos = i;
    return k; // Return length of variable name
}

// Safely resize result buffer - ensures no memory leaks
static char *safe_resize_buffer(char *buffer, size_t *capacity, size_t needed)
{
    size_t new_capacity;
    char *new_buffer;

    if (needed < *capacity)
        return (buffer);
    new_capacity = *capacity;
    while (new_capacity <= needed)
        new_capacity *= 2;
    new_buffer = realloc(buffer, new_capacity);
    if (!new_buffer)
        return (NULL);
    *capacity = new_capacity;
    return (new_buffer);
}

// Helper function to handle expansion of special variables
static int handle_special_var(t_minishell *mini, char **result,
                              size_t *capacity, int *j, const char *str, int *i)
{
    char status_str[12];
    char *temp;
    size_t len;

    if (str[*i] == '?')
    {
        (*i)++;
        int_to_str(mini->exit_status, status_str);
        len = ft_strlen(status_str);
        temp = safe_resize_buffer(*result, capacity, *j + len + 1);
        if (!temp)
            return (-1);
        *result = temp;
        simple_strcat(*result + *j, status_str);
        *j += len;
        return (1);
    }
    else if (str[*i] == '$')
    {
        (*i)++;
        int_to_str(getpid(), status_str);
        len = ft_strlen(status_str);
        temp = safe_resize_buffer(*result, capacity, *j + len + 1);
        if (!temp)
            return (-1);
        *result = temp;
        simple_strcat(*result + *j, status_str);
        *j += len;
        return (1);
    }
    return (0);
}

// Helper to handle regular variable expansion
static int handle_variable_expansion(t_minishell *mini, const char *str,
                                     char **result, size_t *capacity, int *i, int *j)
{
    char var[256];
    int is_braced;
    int var_len;
    const char *val;
    char *temp;
    size_t len;

    ft_memset(var, 0, sizeof(var));
    var_len = extract_var_name(str, i, var, &is_braced);
    if (var_len < 0 || var_len == 0)
    {
        temp = safe_resize_buffer(*result, capacity, *j + 2);
        if (!temp)
            return (-1);
        *result = temp;
        (*result)[(*j)++] = '$';
        (*result)[*j] = '\0';
        return (0);
    }

    // Handle special variables in braced syntax
    if (var_len == 1 && var[0] == '?')
    {
        char status_str[12];
        int_to_str(mini->exit_status, status_str);
        len = ft_strlen(status_str);
        temp = safe_resize_buffer(*result, capacity, *j + len + 1);
        if (!temp)
            return (-1);
        *result = temp;
        simple_strcat(*result + *j, status_str);
        *j += len;
        return (0);
    }
    else if (var_len == 1 && var[0] == '$')
    {
        char pid_str[12];
        int_to_str(getpid(), pid_str);
        len = ft_strlen(pid_str);
        temp = safe_resize_buffer(*result, capacity, *j + len + 1);
        if (!temp)
            return (-1);
        *result = temp;
        simple_strcat(*result + *j, pid_str);
        *j += len;
        return (0);
    }

    val = get_value_env(mini, var);
    if (!val)
        val = "";
    len = ft_strlen(val);
    temp = safe_resize_buffer(*result, capacity, *j + len + 1);
    if (!temp)
        return (-1);
    *result = temp;
    simple_strcat(*result + *j, val);
    *j += len;
    return (0);
}

// Process the entire string for variable expansion
static int process_string_expansion(t_minishell *mini, const char *str,
                                    char **result, size_t *capacity, int *i, int *j)
{
    char *temp;
    int special_result;

    while (str[*i])
    {
        // Check for escaped $ (marked with \x01$)
        if (str[*i] == '\x01' && str[*i + 1] == '$')
        {
            // This is an escaped $, add literal $ and skip marker
            temp = safe_resize_buffer(*result, capacity, *j + 2);
            if (!temp)
                return (-1);
            *result = temp;
            (*result)[(*j)++] = '$'; // Add literal $
            (*result)[*j] = '\0';
            *i += 2; // Skip both marker and $
        }
        else if (str[*i] == '$' && str[*i + 1])
        {
            (*i)++;
            special_result = handle_special_var(mini, result, capacity, j, str, i);
            if (special_result == -1)
                return (-1);
            if (special_result == 0)
                if (handle_variable_expansion(mini, str, result, capacity, i, j) == -1)
                    return (-1);
        }
        else
        {
            temp = safe_resize_buffer(*result, capacity, *j + 2);
            if (!temp)
                return (-1);
            *result = temp;
            (*result)[(*j)++] = str[(*i)++];
            (*result)[*j] = '\0';
        }
    }
    return (0);
}

// Get user's home directory from passwd database when HOME is unset
static char *get_user_home_dir(void)
{
    struct passwd *pw = getpwuid(getuid());
    if (pw && pw->pw_dir)
        return ft_strdup(pw->pw_dir);
    return NULL;
}

// Expand tilde at start (~ or ~/)
static char *expand_tilde(t_minishell *minishell, const char *str)
{
    if (str[0] == '~' && (str[1] == '/' || str[1] == '\0'))
    {
        const char *home = get_value_env(minishell, "HOME");
        char *home_to_free = NULL;
        
        // If HOME is unset, get home directory from passwd database
        if (!home)
        {
            home_to_free = get_user_home_dir();
            home = home_to_free;
            if (!home)
                home = ""; // Fallback to empty string if passwd lookup fails
        }
        
        size_t home_len = ft_strlen(home);
        size_t rest_len = ft_strlen(str + 1); // skip '~'
        char *result = malloc(home_len + rest_len + 1);
        if (!result)
        {
            if (home_to_free)
                free(home_to_free);
            return NULL;
        }
        ft_memcpy(result, home, home_len);
        ft_memcpy(result + home_len, str + 1, rest_len);
        result[home_len + rest_len] = '\0';
        
        if (home_to_free)
            free(home_to_free);
        return result;
    }
    else
    {
        return ft_strdup(str);
    }
}

// Main variable replacement function
// char *replace_var(t_minishell *minishell, const char *str, char quote)
// {
//     size_t capacity;
//     char *result;
//     int i;
//     int j;

//     if (quote == '\'')
//         return (ft_strdup(str));
//     // First, expand tilde if any
//     char *tilde_expanded = expand_tilde(minishell, str);
//     if (!tilde_expanded)
//         return NULL;
//     capacity = 1024;
//     result = malloc(capacity);
//     if (!result)
//         return (NULL);
//     result[0] = '\0';
//     i = 0;
//     j = 0;
//     if (process_string_expansion(minishell, str, &result, &capacity, &i, &j) == -1)
//     {
//         free(result);
//         return (NULL);
//     }
//     result[j] = '\0';
//     return (result);
// }

char *replace_var(t_minishell *minishell, const char *str, char quote)
{
    size_t capacity;
    char *result;
    int i;
    int j;

    if (quote == '\'')
        return (ft_strdup(str));

    // Expand tilde first
    char *tilde_expanded = expand_tilde(minishell, str);
    if (!tilde_expanded)
        return NULL;

    capacity = 1024;
    result = malloc(capacity);
    if (!result)
    {
        free(tilde_expanded);
        return NULL;
    }
    result[0] = '\0';
    i = 0;
    j = 0;

    // Use tilde_expanded (not str) for variable expansion
    if (process_string_expansion(minishell, tilde_expanded, &result, &capacity, &i, &j) == -1)
    {
        free(result);
        free(tilde_expanded);
        return NULL;
    }

    result[j] = '\0';
    free(tilde_expanded);  // Free temporary tilde expanded string
    return (result);
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


