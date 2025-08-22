/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 22:25:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/22 23:53:39 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	print_env_error(t_minishell *shell, char *cmd, int error_code,
			char *error_msg)
{
	ft_putstr_fd("./minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->exit_status = error_code;
}

void	handle_dir_path(t_minishell *shell, char *cmd, char *orig_name)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
		print_env_error(shell, orig_name, 127, "No such file or directory");
	else if (stat(cmd, &st) == 0)
	{
		if (S_ISDIR(st.st_mode))
			print_env_error(shell, orig_name, 126, "Permission denied");
		else
			print_env_error(shell, orig_name, 126, "Not a directory");
	}
	else
		print_env_error(shell, orig_name, 127, "No such file or directory");
}

void	handle_file_path(t_minishell *shell, char *cmd)
{
	struct stat	st;

	if (access(cmd, F_OK) != 0)
		print_env_error(shell, cmd, 127, "No such file or directory");
	else if (stat(cmd, &st) == 0)
	{
		if (access(cmd, X_OK) != 0)
			print_env_error(shell, cmd, 126, "Permission denied");
		else
			shell->exit_status = 0;
	}
	else
		print_env_error(shell, cmd, 127, "No such file or directory");
}
