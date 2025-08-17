/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_dot.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 16:40:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 17:05:08 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	call_dot(t_minishell *mini, char **argv)
{
	if (!argv[1])
	{
		ft_putstr_fd("minishell: .: filename argument required\n", STDERR_FILENO);
		mini->exit_status = 2;
		return ;
	}
	ft_putstr_fd("minishell: .: ", STDERR_FILENO);
	ft_putstr_fd(argv[1], STDERR_FILENO);
	ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
	mini->exit_status = 1;
}
