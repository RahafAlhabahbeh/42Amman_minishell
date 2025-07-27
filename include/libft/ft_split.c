/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/09 11:21:18 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/09 12:45:28 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Skip any separators
// We found a word
// Skip the rest of the word

#include "libft.h"

static int	ft_count_words(char const *s, char sep)
{
	int	word_count;

	word_count = 0;
	while (*s)
	{
		while (*s == sep)
			++s;
		if (*s)
			++word_count;
		while (*s && *s != sep)
			++s;
	}
	return (word_count);
}

static void	ft_free_split_recursive(char **split, size_t index)
{
	if (split[index] != NULL)
	{
		free(split[index]);
		ft_free_split_recursive(split, index + 1);
	}
	else
	{
		free(split);
	}
}

static int	length(char const *s, char c)
{
	if (!ft_strchr(s, c))
		return (ft_strlen(s));
	return (ft_strchr(s, c) - s);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	**aos;

	aos = ft_calloc(ft_count_words(s, c) + 1, sizeof(char *));
	if (!aos || !s)
		return (NULL);
	i = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
		{
			aos[i] = ft_substr(s, 0, length(s, c));
			if (!aos[i])
			{
				ft_free_split_recursive(aos, 0);
				return (NULL);
			}
			s += length(s, c);
			i++;
		}
	}
	aos[i] = NULL;
	return (aos);
}
/*int main() {
	char **res = ft_split("   Hi this is me ", ' ');
	if (res){
		for (size_t i = 0; res[i]; i++){
			printf("%s\n", res[i]);		
}
}
return 0;
}*/
