/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_op2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:17:47 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:17:53 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redir_op2(char **tokens, int *k, const char *input, int *i)
{
    if (input[*i + 1] == '>')
    {
        tokens[*k++] = ft_strdup(">>");
        i += 2;
    }
    else
    {
        tokens[*k++] = (">");
        i++;
    }
}