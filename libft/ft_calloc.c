/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/05 17:35:52 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/05 17:36:05 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//size_t nmemb:This is the number of
//elements you want to allocate memory for
//size_t size: This is the size of each element
//When multiplying two large integers, the result
//may exceed the maximum value that can be represented by the integer type

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	bytes;
	void	*ptr;

	bytes = nmemb * size;
	if (size && ((bytes / size) != nmemb))
		return (NULL);
	ptr = malloc(bytes);
	if (NULL == ptr)
		return (NULL);
	ft_bzero(ptr, bytes);
	return (ptr);
}
/*#include <stdio.h>
#include <stdlib.h>
int main() {
    size_t num_elements = 5; // Number of elements to allocate
    size_t element_size = sizeof(int); // Size of each element (e.g., int)

    // Use ft_calloc to allocate and initialize memory
    int *array = (int *)ft_calloc(num_elements, element_size);
    if (array == NULL) {
        perror("ft_calloc failed");
        return EXIT_FAILURE;
    }

    // Print the contents of the allocated memory
    printf("Contents of allocated memory:\n");
    for (size_t i = 0; i < num_elements; ++i) {
        printf("array[%zu] = %d\n", i, array[i]); // Should print zeros
    }

    // Free the allocated memory
    free(array);

    return EXIT_SUCCESS;
}*/
