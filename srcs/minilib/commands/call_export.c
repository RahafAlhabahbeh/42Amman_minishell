/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/04 09:34:15 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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
                while (i-- > 0) free(arr[i]);
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
                while (i-- > 0) free(arr[i]);
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

static void export_with_args(t_minishell *mini, char **argv)
{
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

void call_export(t_minishell *mini, char **argv)
{
    if (!argv[1])
        export_no_args(mini);
    else
        export_with_args(mini, argv);
}
