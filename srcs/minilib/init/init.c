/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/20 14:10:43 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	init(t_minishell *mini)
{
	mini->promp_input = NULL;
	mini->cmd = NULL;
	mini->cmd_count = 0;
	mini->token_count = 0;
	mini->pipex_count = 0;
	mini->token = NULL;
	mini->fd_out = 1;
	mini->env_list = NULL;
	mini->exit_status = 0;
	mini->child_env = NULL;
	ft_memset(mini->buff, 0, sizeof(mini->buff));
}
