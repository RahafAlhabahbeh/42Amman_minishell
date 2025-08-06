/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 20:09:19 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/02 20:40:24 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//it is similar to memcpy but is designed to
//handle overlapping memory regions safely.
//Byte-wise copying
//If the destination is ahead of the source in memory,
//copying is done backwards to prevent
//overwriting data that hasn’t been copied yet.
//j = 1;: Initialize j to 1.
//This variable will control the direction of copying.
//-1 means  backwards
//move the ponter to start copying from the end

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			i;
	char			j;
	unsigned char	*d;
	unsigned char	*s;

	i = 0;
	j = 1;
	d = (unsigned char *)dest;
	s = (unsigned char *)src;
	if (!dest && !src)
		return (dest);
	if (dest > src)
	{
		j = -1;
		d += n - 1;
		s += n - 1;
	}
	while (i < n)
	{
		*d = *s;
		d += j;
		s += j;
		i++;
	}
	return (dest);
}
/*
int main()
{
	char	src[] = "lorem ipsum dolor sit amet";
	char    *dest;

	dest = src + 1;
	printf("%s\n", (char *)ft_memmove(src, dest, 8));
}
*/
