/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_op1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:17:12 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:17:17 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void redir_op1(char **tokens, int *k, const char *input, int *i)
{
    if (input[*i] == '<')
    {
        if (input[*i + 1] == '<')
        {
            tokens[*k++] = ft_strdup("<<");
            i += 2;
        }
        else
        {
            tokens[*k++] = ft_strdup("<");
            i++;
        }
    }
    else if (input[*i] == '>')
        redir_op2(tokens, k, input, i);
}