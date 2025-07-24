/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:53:38 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/07/24 08:54:30 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_numeric(const char *str)
{
	int i = 0;

	if (!str || str[0] == '\0')
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	call_exit(t_minishell *mini, char **argv)
{
	long	code;

	ft_putstr_fd("exit\n", STDERR_FILENO);
	if (!argv[1])
		exit(mini->exit_status);
	if (!is_numeric(argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd(": numeric argument required\n", STDERR_FILENO);
		exit(255);
	}
	if (argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", STDERR_FILENO);
		mini->exit_status = 1;
		return;
	}
	code = ft_atoi(argv[1]);
	exit((unsigned char)code); // exit with lowest 8 bits, like bash
}



/*

dal-mahr@c1r6s5:~/Desktop/minihell$ jdfkl
jdfkl: command not found
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
127
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ls
include  libft	Makefile  minishell  Notes  obj  out.txt  readline.supp  srcs
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
0
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ jdfslk
jdfslk: command not found
dal-mahr@c1r6s5:~/Desktop/minihell$ exit
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
127
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ls jfdlk
ls: cannot access 'jfdlk': No such file or directory
dal-mahr@c1r6s5:~/Desktop/minihell$
exit
dal-mahr@c1r6s5 ~/Desktop/minihell
 % echo $?
2
dal-mahr@c1r6s5 ~/Desktop/minihell
 % bash
dal-mahr@c1r6s5:~/Desktop/minihell$ ^C
dal-mahr@c1r6s5:~/Desktop/minihell$ echo $?
130
dal-mahr@c1r6s5:~/Desktop/minihell$ cat
^\Quit (core dumped)
dal-mahr@c1r6s5:~/Desktop/minihell$ echo $?
131
dal-mahr@c1r6s5:~/Desktop/minihell$
*/
