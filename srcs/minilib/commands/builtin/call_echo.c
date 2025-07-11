/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/09 07:09:34 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/09 07:10:12 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int check_error(t_minishell *minishell, int i)
{

}

char *call_subshell(t_minishell *minishell, int i, int start_index, int end_index)
{
	char *message = (char *) malloc (sizeof(char) * );
	while (minishell->cmd[i])
	{
		
	}
}

int quote(t_minishell *minishell, int i, int quote_flag)
{
	int error = 0;
	error = check_error(t_minishell *minishell, int i);
	if (!error)
		return error; // there are an error
	
	
	// there are a sub shell
	// echo -n "$(ls)"
	// echo "hi from sub shell `ls`"
	char *message;
	int len = 0;
	int start_index = 0;
	int end_index = ft_strlen(minishell->cmd[i]);

	if (minishell->cmd[i][start_index] == '\"')
	{
		
		while (minishell->cmd[i][end_index] != '\"')
			end_index--;
		message = call_subshell(minishell, i, start_index, end_index);
	}
}

void call_echo(t_minishell *minishell, int op)
{
    int fd;
    int i;
    int quote_flag;
    quote_flag = 0;
	
    fd = minishell->fd_out;
    if (fd == -1)
	fd = 1;
    i = 1;
    if (op)
    	i++;
    	
	int err = quote(minishell, i, quote_flag);
    while (minishell->cmd[i])
    {
	    if (write(fd, minishell->cmd[i], ft_strlen(minishell->cmd[i])) == -1)
	    {
	    	printf("error\n");
		perror("ECHO error\n");
		exit(1);
	    }
	    i++;
	    if (!minishell->cmd[i])
	    	break;
	    write(fd, " ", 1);
    }
    if (!op)
        write(fd, "\n", 1);
}
