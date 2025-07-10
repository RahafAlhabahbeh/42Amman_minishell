/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   normal_string.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:14:49 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:14:59 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void normal_string(char **tokens, int *k, const char *input, int *i)
{
    char *tok;
    int size;
    int start = *i;

    while (input[*i] && input[*i] != ' ' 
        && input[*i] != '\'' && input[*i] != '"' 
        && input[*i] != '>' && input[*i] != '<' 
        && input[*i] != '|')
        (*i)++;

    size = *i - start;
    tok = malloc(size + 1);
    ft_strlcpy(tok, &input[start], size + 1);
    tokens[(*k)++] = tok;
}