/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:38 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/02 22:14:39 by aaljazza         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void init(t_minishell *shell)
{
	shell->i = 0;
	shell->j = 0;
	shell->fd_in = -1;
	shell->fd_out = -1;
	shell->fd_app = -1;
	shell->tok = NULL;
}
