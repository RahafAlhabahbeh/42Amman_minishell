/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:18:53 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/03 17:43:30 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//compare two blocks of memory
//current position i in s1 and s2 are different
//This difference will be a positive or 
//negative integer depending on which byte is greater

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		if (*((unsigned char *)s1 + i) != *((unsigned char *)s2 + i))
			return (*((unsigned char *)s1 + i) - *((unsigned char *)s2 + i));
		++i;
	}
	return (0);
}
/*#include<stdio.h>

int main()
{
printf("%d",ft_memcmp("rahaf","ra",6));
return (0);
}*/
