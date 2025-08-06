/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 14:36:16 by ral-haba          #+#    #+#             */
/*   Updated: 2024/09/20 15:57:43 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*deleting all its nodes and
freeing the associated memory
Check if the list and delete function are valid 
Start from the head of the list 
Save the next node
 Delete the current node
 Move to the next node
 Set the list head to NULL*/
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void*))
{
	t_list	*temp;
	t_list	*prev;

	if (lst && *lst && del)
	{
		prev = (t_list *)*lst;
		while (prev)
		{
			temp = prev->next;
			ft_lstdelone(prev, del);
			prev = temp;
		}
		*lst = NULL;
	}
}
/*void del_content(void *content) {
    // Assuming content is dynamically allocated memory
    free(content);
}

int main() {
    // Create a linked list with some sample data
    t_list *list = ft_lstnew(malloc(sizeof(int))); // Sample integer
    *(int *)(list->content) = 42; // Assign value

    list->next = ft_lstnew(malloc(sizeof(int))); // Next node
    *(int *)(list->next->content) = 84; // Assign value

    list->next->next = ft_lstnew(malloc(sizeof(int))); // Next node
    *(int *)(list->next->next->content) = 168;
    // Clear the list
    ft_lstclear(&list, del_content);

   
    if (list == NULL) {
        printf("List successfully cleared.\n");
    } else {
        printf("List not cleared.\n");
    }

    return 0;
}*/
