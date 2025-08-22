/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	print_env_vars(t_minishell *shell)
{
	t_env	*curr;

	curr = shell->env_list;
	while (curr)
	{
		if (curr->value)
		{
			ft_putstr_fd(curr->key, STDOUT_FILENO);
			ft_putchar_fd('=', STDOUT_FILENO);
			ft_putstr_fd(curr->value, STDOUT_FILENO);
			ft_putchar_fd('\n', STDOUT_FILENO);
		}
		curr = curr->next;
	}
}

static void	execute_env_command(t_minishell *shell, char **argv)
{
	char	*cmd;
	int		arg_idx;

	arg_idx = 1;
	if (argv[1] && ft_strcmp(argv[1], "+") == 0)
		arg_idx = 2;
	cmd = argv[arg_idx];
	if (!cmd)
	{
		shell->exit_status = 0;
		return ;
	}
	if (cmd[ft_strlen(cmd) - 1] == '/')
	{
		cmd[ft_strlen(cmd) - 1] = '\0';
		handle_dir_path(shell, cmd, argv[arg_idx]);
	}
	else
		handle_file_path(shell, cmd);
}

void	call_env(t_minishell *shell, char **argv)
{
	if (!argv[1])
	{
		print_env_vars(shell);
		shell->exit_status = 0;
	}
	else
	{
		execute_env_command(shell, argv);
	}
}
