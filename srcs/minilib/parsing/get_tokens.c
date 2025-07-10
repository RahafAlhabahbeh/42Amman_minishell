/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_tokens.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/03 00:52:12 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/03 00:52:32 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


char **get_tokens(const char *input)
{
    size_t len = ft_strlen(input);
    char **tokens = (char **)malloc(sizeof(char *) * (len + 1));
    int i = 0, k = 0;

    while (input[i])
    {
        while (input[i] == ' ')
            i++;
        if (!input[i])
            break;
        if (input[i] == '|')
        {
            tokens[k++] = ft_strdup("|");
            i++;
        }
        else if (input[i] == '<' || input[i] == '>')
            redir_op1(tokens, &k, input, &i);
        else
        {
            quoted(tokens, &k, input, &i);
        }
    }
    tokens[k] = NULL;
    return tokens;
}
