/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getline.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/16 20:40:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/16 20:40:00 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static int	ft_getline_resize(char **lineptr, size_t *n, size_t pos)
{
	size_t	new_size;
	char	*new_buf;

	new_size = *n * 2;
	new_buf = malloc(new_size);
	if (!new_buf)
		return (-1);
	ft_memcpy(new_buf, *lineptr, pos);
	free(*lineptr);
	*lineptr = new_buf;
	*n = new_size;
	return (0);
}

static int	ft_getline_init(char **lineptr, size_t *n)
{
	if (!lineptr || !n)
		return (-1);
	if (!*lineptr || *n == 0)
	{
		*lineptr = malloc(128);
		if (!*lineptr)
			return (-1);
		*n = 128;
	}
	return (0);
}

static ssize_t	ft_getline_loop(char **lineptr, size_t *n, int fd)
{
	char	c;
	ssize_t	bytes_read;
	size_t	pos;

	pos = 0;
	bytes_read = 1;
	while (bytes_read > 0)
	{
		bytes_read = read(fd, &c, 1);
		if (bytes_read <= 0)
			break ;
		if (pos >= *n - 1)
			if (ft_getline_resize(lineptr, n, pos) < 0)
				return (-1);
		(*lineptr)[pos++] = c;
		if (c == '\n')
			break ;
	}
	if (pos == 0 && bytes_read == 0)
		return (-1);
	(*lineptr)[pos] = '\0';
	return ((ssize_t)pos);
}

ssize_t	ft_getline(char **lineptr, size_t *n, int fd)
{
	if (ft_getline_init(lineptr, n) < 0)
		return (-1);
	return (ft_getline_loop(lineptr, n, fd));
}
