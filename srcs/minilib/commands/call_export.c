#include "../../../include/minishell.h"

// Print export list sorted (declare -x ...)
// Variables with NULL value: print without =""
// Variables with value (including empty string): print with =""
void print_export_list(t_env *env_list)
{
    // Count variables
    int count = 0;
    t_env *cur = env_list;
    while (cur)
    {
        count++;
        cur = cur->next;
    }

    // Create array of strings: "key" or "key=value"
    char **arr = malloc(sizeof(char *) * count);
    if (!arr)
        return;

    cur = env_list;
    for (int i = 0; i < count; i++)
    {
        if (cur->value == NULL)
            arr[i] = ft_strdup(cur->key);
        else
            arr[i] = malloc(ft_strlen(cur->key) + 2 + ft_strlen(cur->value) + 1); // key + =" + value + \0
        if (cur->value != NULL)
            sprintf(arr[i], "%s=\"%s\"", cur->key, cur->value);
        cur = cur->next;
    }

    // Simple bubble sort by key
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if (strcmp(arr[j], arr[j + 1]) > 0)
            {
                char *tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }

    // Print
    for (int i = 0; i < count; i++)
    {
        if (ft_strchr(arr[i], '='))
            printf("declare -x %s\n", arr[i]);
        else
            printf("declare -x %s\n", arr[i]);
        free(arr[i]);
    }
    free(arr);
}

// Print env list for env command (only variables with value != NULL)
void print_env_list(t_env *env_list)
{
    t_env *cur = env_list;
    while (cur)
    {
        if (cur->value != NULL)
            printf("%s=%s\n", cur->key, cur->value);
        cur = cur->next;
    }
}

// Validate identifier for export (letters, digits, underscores, first char letter or underscore)
int is_valid_identifier(const char *s) 
{
    if (!s)
        return 0;
    if (!((s[0] >= 'A' && s[0] <= 'Z') || 
          (s[0] >= 'a' && s[0] <= 'z') || 
          s[0] == '_'))
        return 0;
    for (int i = 1; s[i] && s[i] != '='; i++) 
    {
        if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ||
              (s[i] >= '0' && s[i] <= '9') || s[i] == '_'))
            return 0;
    }
    return 1;
}

// Helper: Compare keys for sorting
static int cmp_env_key(const void *a, const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;
    // Compare keys only (up to '=' or end)
    int i = 0;
    while (s1[i] && s1[i] != '=' && s2[i] && s2[i] != '=' && s1[i] == s2[i])
        i++;
    if ((s1[i] == '=' || s1[i] == '\0') && (s2[i] == '=' || s2[i] == '\0'))
        return 0;
    if (s1[i] == '=' || s1[i] == '\0')
        return -1;
    if (s2[i] == '=' || s2[i] == '\0')
        return 1;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void call_export(t_minishell *mini, char **argv)
{
    if (!argv[1]) // no args, print sorted export list
    {
        int count = 0;
        t_env *cur = mini->env_list;
        while (cur)
        {
            count++;
            cur = cur->next;
        }

        char **arr = malloc(sizeof(char *) * count);
        if (!arr)
            return; // handle malloc failure

        cur = mini->env_list;
        for (int i = 0; i < count; i++)
        {
            if (cur->value == NULL)
            {
                arr[i] = ft_strdup(cur->key);
            }
            else
            {
                int len = ft_strlen(cur->key) + ft_strlen(cur->value) + 2;
                arr[i] = malloc(len);
                if (!arr[i])
                {
                    for (int j = 0; j < i; j++)
                        free(arr[j]);
                    free(arr);
                    return;
                }
                ft_strlcpy(arr[i], cur->key, len);
                ft_strlcat(arr[i], "=", len);
                ft_strlcat(arr[i], cur->value, len);
            }
            cur = cur->next;
        }


        // Sort env strings by key
        qsort(arr, count, sizeof(char *), cmp_env_key);

        // Print formatted output
        for (int i = 0; i < count; i++)
        {
            char *eq = ft_strchr(arr[i], '=');
            if (!eq)
            {
                // No value case
                printf("declare -x %s\n", arr[i]);
            }
            else
            {
                // Empty string value (e.g. AAA=) prints as declare -x AAA=""
                if (*(eq + 1) == '\0')
                    printf("declare -x %.*s=\"\"\n", (int)(eq - arr[i]), arr[i]);
                else
                    printf("declare -x %.*s=\"%s\"\n", (int)(eq - arr[i]), arr[i], eq + 1);
            }
            free(arr[i]);
        }
        free(arr);
        return;
    }

    // With args: update env as before (handle keys and values)
    for (int i = 1; argv[i]; i++)
    {
        if (!is_valid_identifier(argv[i]))
        {
            fprintf(stderr, "export: `%s`: not a valid identifier\n", argv[i]);
            continue;
        }

        char *eq = ft_strchr(argv[i], '=');
        if (!eq)
        {
            // No '=': set key with NULL value
            char *key = ft_strdup(argv[i]);
            mini->env_list = set_env_value(mini, key, NULL);
            free(key);
        }
        else
        {
            int key_len = eq - argv[i];
            char *key = ft_substr(argv[i], 0, key_len);
            char *value = ft_strdup(eq + 1);
            mini->env_list = set_env_value(mini, key, value);
            free(key);
            free(value);
        }
    }
}