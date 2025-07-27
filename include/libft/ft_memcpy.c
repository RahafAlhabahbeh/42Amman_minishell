/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 19:48:17 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/02 20:04:48 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//second if to insure that there is no overlap
//Uses unsigned char for byte-by-byte copying to handle arbitrary data correctly
//"Arbitrary data" refers to any kind of data stored in memory,
//not restricted by type or format
#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (NULL);
	if (dest != src)
	{
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	return (dest);
}
