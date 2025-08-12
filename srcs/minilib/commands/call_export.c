/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 06:03:27 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

// static char *remove_outer_quotes(const char *s)
// {
//     size_t len = ft_strlen(s);
//     char *res = malloc(len + 1);
//     if (!res)
//         return NULL;

//     size_t j = 0;
//     for (size_t i = 0; i < len; i++)
//     {
//         if (s[i] != '\'' && s[i] != '"')
//             res[j++] = s[i];
//     }
//     res[j] = '\0';
//     return res;
// }

// int is_valid_identifier(const char *s)
// {
//     if (!s)
//         return 0;
//     if (!((s[0] >= 'A' && s[0] <= 'Z') ||
//           (s[0] >= 'a' && s[0] <= 'z') ||
//           s[0] == '_'))
//         return 0;
//     for (int i = 1; s[i] && s[i] != '='; i++)
//     {
//         if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') ||
//               (s[i] >= '0' && s[i] <= '9') || s[i] == '_'))
//             return 0;
//     }
//     return 1;
// }

int is_valid_identifier(const char *name)
{
    int i = 0;
    if (!name || !name[0]) // empty string
        return 0;
    if (!(ft_isalpha(name[0]) || name[0] == '_'))
        return 0;
    while (name[i] && name[i] != '=')
    {
        if (!(ft_isalnum(name[i]) || name[i] == '_'))
            return 0;
        i++;
    }
    return 1;
}

static int cmp_env_key(const void *a, const void *b)
{
    char *s1 = *(char **)a;
    char *s2 = *(char **)b;
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

static void export_print_sorted_env(char **arr, int count)
{
    qsort(arr, count, sizeof(char *), cmp_env_key);
    for (int i = 0; i < count; i++)
    {
        char *eq = ft_strchr(arr[i], '=');
        if (!eq)
            printf("declare -x %s\n", arr[i]);
        else if (*(eq + 1) == '\0')
            printf("declare -x %.*s=\"\"\n", (int)(eq - arr[i]), arr[i]);
        else
            printf("declare -x %.*s=\"%s\"\n", (int)(eq - arr[i]), arr[i], eq + 1);
        free(arr[i]);
    }
    free(arr);
}

static void export_no_args(t_minishell *mini)
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
        return;
    cur = mini->env_list;
    for (int i = 0; i < count; i++)
    {
        if (!cur->value)
        {
            arr[i] = ft_strdup(cur->key);
            if (!arr[i])
            {
                while (i-- > 0)
                    free(arr[i]);
                free(arr);
                return;
            }
        }
        else
        {
            int len = ft_strlen(cur->key) + ft_strlen(cur->value) + 2;
            arr[i] = malloc(len);
            if (!arr[i])
            {
                while (i-- > 0)
                    free(arr[i]);
                free(arr);
                return;
            }
            ft_strlcpy(arr[i], cur->key, len);
            ft_strlcat(arr[i], "=", len);
            ft_strlcat(arr[i], cur->value, len);
        }
        cur = cur->next;
    }
    export_print_sorted_env(arr, count);
}

char *merge_args(char **argv, int start, int *consumed)
{
    *consumed = 1;
    char *res = ft_strdup(argv[start]);

    for (int i = start + 1; argv[i]; i++)
    {
        // Only merge if this arg came from same logical word (e.g., empty string after '=')
        if (argv[i][0] == '\0')
        {
            char *tmp = ft_strjoin(res, argv[i]);
            free(res);
            res = tmp;
            (*consumed)++;
        }
        else
            break;
    }
    return res;
}

static void export_with_args(t_minishell *mini, char **argv)
{
    int has_error = 0;
    for (int i = 1; argv[i];)
    {
        int consumed;
        char *merged = merge_args(argv, i, &consumed);

        if (!is_valid_identifier(merged))
        {
            fprintf(stderr, "export: `%s`: not a valid identifier\n", merged);
            has_error = 1;
            free(merged);
            i += consumed; // skip all merged parts
            continue;
        }

        char *eq = ft_strchr(merged, '=');
        if (!eq)
            mini->env_list = set_env_value(mini, merged, NULL);
        else
            mini->env_list = set_env_value(
                mini,
                ft_substr(merged, 0, eq - merged),
                ft_strdup(eq + 1));

        free(merged);
        i += consumed; // skip all merged parts
    }

    if (has_error)
        mini->exit_status = 1;
}

// static void export_with_args(t_minishell *mini, char **argv)
// {
//     int has_error = 0;
//     for (int i = 1; argv[i]; i++)
//     {
//         char *clean = remove_outer_quotes(argv[i]);
//         if (!clean)
//             continue;

//         if (!is_valid_identifier(argv[i]))
//         {
//             write(2, "export: `", 9);
//             write(2, argv[i], ft_strlen(argv[i]));
//             write(2, "`: not a valid identifier\n", 26);
//             has_error = 1;
//             continue;
//         }
//         char *eq = ft_strchr(argv[i], '=');
//         if (!eq)
//         {
//             char *key = ft_strdup(argv[i]);
//             mini->env_list = set_env_value(mini, key, NULL);
//             free(key);
//         }
//         else
//         {
//             int key_len = eq - argv[i];
//             char *key = ft_substr(argv[i], 0, key_len);
//             char *value = ft_strdup(eq + 1);
//             mini->env_list = set_env_value(mini, key, value);
//             free(key);
//             free(value);
//         }
//     }
//     if (has_error)
//         mini->exit_status = 1;
// }

void call_export(t_minishell *mini, char **argv)
{
    if (!argv[1])
        export_no_args(mini);
    else
        export_with_args(mini, argv);
}
