/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoted.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:15:41 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:15:51 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void quoted(char **tokens, int *k, const char *input, int *i)
{
    int size;
    char *tok;
    char quote;

    if (input[*i] == '\'' || input[*i] == '"')
    {
        quote = input[(*i)++];
        int start = *i;
        while (input[*i] && input[*i] != quote)
            (*i)++;
        if (!input[*i])
            return ;
        size = *i - start;
        tok = malloc(size + 1);
        ft_strlcpy(tok, &input[start], size + 1);
        tokens[(*k)++] = tok;
        (*i)++;
    }
    else
    {
        normal_string(tokens, k, input, i);
    }
}