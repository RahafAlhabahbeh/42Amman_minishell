/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: snazzal <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 10:42:08 by snazzal           #+#    #+#             */
/*   Updated: 2024/09/08 10:42:29 by snazzal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
#include <limits.h>

static unsigned int	nblen(int n)
{
	unsigned int	i;

	i = 0;
	if (n < 0)
		i++;
	if (n == 0)
		return (1);
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	unsigned int	len;
	char			*arr;
	long			nb;

	len = nblen(n);
	arr = malloc((len + 1) * sizeof(char));
	nb = n;
	if (!arr)
		return (NULL);
	arr[len--] = '\0';
	if (nb == 0)
		arr[0] = '0';
	if (nb < 0)
	{
		arr[0] = '-';
		nb = -nb;
	}
	while (nb != 0)
	{
		arr[len--] = '0' + (nb % 10);
		nb /= 10;
	}
	return (arr);
}

/*int main() {
    int numbers[] = {0, 1, -1, 123, -123, INT_MAX, INT_MIN};
    int num_tests = sizeof(numbers) / sizeof(numbers[0]);

    for (int i = 0; i < num_tests; i++) {
        char *str = ft_itoa(numbers[i]);
        if (str) {
            printf("Integer: %d, String: %s\n", numbers[i], str);
        } else {
            printf("Memory allocation failed for integer: %d\n", numbers[i]);
        }
    }

    return 0;
}*/
