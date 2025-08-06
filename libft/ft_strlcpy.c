/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/03 17:53:08 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/03 18:32:10 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Copy the string pointed to by src, 
//including the terminating null byte ('\0'),
//to the buffer pointed to by dst
//The size - 1 ensures that there is room for 
//the null terminator ('\0') in the destination buffer

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	len;

	len = ft_strlen(src);
	if (size < 1)
		return (len);
	i = 0;
	while (src[i] != '\0' && i < (size - 1))
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (len);
}
