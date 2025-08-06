/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/02 12:01:25 by ral-haba          #+#    #+#             */
/*   Updated: 2024/07/02 12:01:35 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//returns a pointer to a new string which is dublicate of the string s
//move to the last elemint in src ||i = ft_strlen(src) + 1;
//allocate dynamic memory debend in src size in ||dest = malloc(i)
//Check if malloc succeeded
//copy the string ||ft_strlcpy(dest, src, i);

#include <stdlib.h>

char	*ft_strdup(char *src)
{
	char	*dest;
	int		i;

	i = 0;
	while (src[i] != '\0')
		i++;
	dest = (char *)malloc(1 + i);
	i = 0;
	if (dest == NULL)
		return (0);
	else
	{
		while (src[i])
		{
			dest[i] = src[i];
			i++;
		}
		dest[i] = '\0';
		return (dest);
	}
}
/*#include<stdio.h>
int main()
{
	char *c;
	c = "Rahaf";
	
	printf("%s",ft_strdup(c));
	return 0;
}*/
