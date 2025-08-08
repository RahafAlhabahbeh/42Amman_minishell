/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/**   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:58 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/24 08:55:05 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void unset_env(t_env **env, const char *key)
{
    t_env *cur = *env;
    t_env *prev = NULL;

    while (cur)
    {
        if (ft_strncmp(cur->key, key, ft_strlen(key)) == 0)
        {
            if (prev)
                prev->next = cur->next;
            else
                *env = cur->next;

            free(cur->key);
            free(cur->value);
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

void call_unset(t_minishell *mini, char **argv)
{
    int has_error = 0;
    for (int i = 1; argv[i]; i++)
    {
        if (!is_valid_identifier(argv[i]))
        {
            write(2, "unset: `", 8);
            write(2, argv[i], ft_strlen(argv[i]));
            write(2, "`: not a valid identifier\n", 26);
            has_error = 1;
            continue;
        }
        unset_env(&mini->env_list, argv[i]);
    }
    if (has_error)
        mini->exit_status = 1;
}
