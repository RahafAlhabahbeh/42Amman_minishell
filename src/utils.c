/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:45:01 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/23 11:12:13 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	open_file(char *filename, int flags, int mode)
{
	int	fd;

	fd = open(filename, flags, mode);
	if (fd == -1)
		perror("open");
	return (fd);
}

void	free_str_array(char **arr)
{
    int	i;

    if (!arr)
        return;
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}

void	ft_free_split_recursive(char **array, int index)
{
	if (!array || !array[index])
	{
		free(array);
		return;
	}
	free(array[index]);
	ft_free_split_recursive(array, index + 1);
}
