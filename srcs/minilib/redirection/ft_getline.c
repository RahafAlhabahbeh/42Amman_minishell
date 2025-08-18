/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/31 01:11:13 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/18 14:30:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	ft_realloc_line(char **lineptr, size_t *n, size_t pos)
{
	char	*new_ptr;
	size_t	i;
	size_t	size;

	size = *n * 2;
	new_ptr = malloc(size);
	if (!new_ptr)
		return (-1);
	i = 0;
	while (i < pos)
	{
		new_ptr[i] = (*lineptr)[i];
		i++;
	}
	free(*lineptr);
	*lineptr = new_ptr;
	*n = size;
	return (0);
}

static int	init_lineptr(char **lineptr, size_t *n)
{
	if (!lineptr || !n)
		return (-1);
	if (!*lineptr || *n == 0)
	{
		*n = 128;
		*lineptr = malloc(*n);
		if (!*lineptr)
			return (-1);
	}
	return (0);
}

ssize_t	ft_getline(char **lineptr, size_t *n, int fd)
{
	char	c;
	ssize_t	bytes_read;
	size_t	pos;

	pos = 0;
	if (init_lineptr(lineptr, n) == -1)
		return (-1);
	bytes_read = read(fd, &c, 1);
	while (bytes_read > 0)
	{
		if (pos >= *n - 1 && ft_realloc_line(lineptr, n, pos) == -1)
			return (-1);
		(*lineptr)[pos++] = c;
		if (c == '\n')
			break ;
		bytes_read = read(fd, &c, 1);
	}
	if (bytes_read < 0 || (pos == 0 && bytes_read == 0))
		return (-1);
	(*lineptr)[pos] = '\0';
	return (pos);
}
