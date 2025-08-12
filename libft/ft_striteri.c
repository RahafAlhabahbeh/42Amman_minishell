/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:12:20 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/08 18:12:29 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char*))
{
	unsigned int	i;

	i = 0;
	while (s[i])
	{
		(*f)(i, &s[i]);
		i++;
	}
}
/*#include <stdio.h>
#include <string.h>

void to_uppercase(unsigned int index, char *c) {
	(void)index;
    if (*c >= 'a' && *c <= 'z') 
	{
        *c = *c - 'a' + 'A';
	}
}

int main() {
    char str[] = "hello world";

    printf("Original: %s\n", str);

    ft_strmapi(str, to_uppercase);

    printf("Modified: %s\n", str);

    return 0;
}*/
