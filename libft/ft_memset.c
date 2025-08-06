/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 17:59:21 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/02 18:02:51 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//pointer to the memory block to be set
//the value to set it to
//the number of bytes to set

//#include<stdio.h>
//#include<string.h>
#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	size_t	i;
	char	*str;

	i = 0;
	str = s;
	while (i < n)
	{
		str[i] = c;
		i++;
	}
	return (s);
}

/*
int	main(void)
{
	char	v[10]; 

	ft_memset(v, 42, 5);

	for (int i = 0; i < 10; ++i)	
		printf("%d\n", v[i]);
	printf("ft_memset-> %p\n\n", ft_memset(v, 42, 5));
	


	memset(v, 21, 5);

	for (int i = 0; i < 10; ++i)
		printf("%d\n", v[i]);
	printf("Real memset-> %p\n", memset(v, 21, 5));
}
*/
