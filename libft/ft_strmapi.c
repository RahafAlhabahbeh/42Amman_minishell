/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmpi.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 17:21:01 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/08 17:47:10 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char	*r;
	int		i;

	i = 0;
	r = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!r)
		return (NULL);
	while (s[i])
	{
		r[i] = (*f)(i, s[i]);
		i++;
	}
	r[i] = '\0';
	return (r);
}

/*
#include <string.h>

char to_uppercase(unsigned int index, char c) {
	(void)index;
    if (c >= 'a' && c <= 'z') {
        return c - 'a' + 'A';
    }
    return c;
}
int main() {
    char str[] = "hello world";
    char *result;

    result = ft_strmapi(str, to_uppercase);

    if (result) {
        printf("Original: %s\n", str);
        printf("Transformed: %s\n", result);
        free(result); // Free the allocated memory
    } else {
        printf("Error in ft_strmapi\n");
    }

    return 0;
}*/
