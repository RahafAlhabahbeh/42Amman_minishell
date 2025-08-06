/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/06 18:08:31 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/06 18:08:57 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//i start j end str
#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	i;
	size_t	j;
	char	*str;

	str = 0;
	if (s1 != 0 && set != 0)
	{
		i = 0;
		j = ft_strlen(s1);
		while (s1[i] && ft_strchr(set, s1[i]))
			i++;
		while (s1[j - 1] && ft_strchr(set, s1[j - 1]) && j > i)
			j--;
		str = (char *)malloc(sizeof(char) * (j - i + 1));
		if (str)
			ft_strlcpy(str, &s1[i], j - i + 1);
	}
	return (str);
}
/*int main() {
    const char *str = "abc cba Hello, World! bca";
    const char *set = "abc ";

    char *trimmed_str = ft_strtrim(str, set);
    if (trimmed_str) {
        printf("Original string: '%s'\n", str);
        printf("Trimmed string: '%s'\n", trimmed_str);
        free(trimmed_str);
    } else {
        printf("Memory allocation failed.\n");
    }

    return 0;
}*/
