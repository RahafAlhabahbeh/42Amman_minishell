/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:21:39 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/19 14:30:10 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//declarating, allocating, malloc prodection
//malloc(sizeof(t_list));
#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*arr_list;

	arr_list = (t_list *)malloc(sizeof(t_list) * 1);
	if (!arr_list)
		return (NULL);
	arr_list->content = content;
	arr_list->next = NULL;
	return (arr_list);
}
/*int main(){
	char	*content = "Rahaf";
	t_list *node = ft_lstnew(content);
	if(node)
	{
		printf("%s\n", (char *)node->content);
		free(node);
	}
	return 0;
}*/
