/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 16:53:41 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/05 17:10:49 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int	num;
	int	isneg;
	int	i;

	num = 0;
	isneg = 1;
	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'
			|| str[i] == '\n' || str[i] == '\r'
			|| str[i] == '\v' || str[i] == '\f'))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		isneg *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		i++;
	}
	return (num * isneg);
}

/*#include <stdio.h>
int main() {
    const char *test_str1 = "   012345";
    const char *test_str2 = "-12345";
    const char *test_str3 = "   +67890";
    const char *test_str4 = "  42abc";

    printf("String: \"%s\" -> Integer: %d\n", test_str1, ft_atoi(test_str1));
    printf("String: \"%s\" -> Integer: %d\n", test_str2, ft_atoi(test_str2));
    printf("String: \"%s\" -> Integer: %d\n", test_str3, ft_atoi(test_str3));
    printf("String: \"%s\" -> Integer: %d\n", test_str4, ft_atoi(test_str4));

    return 0;
}*/
