/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/21 20:24:00 by rahaf            ###   ########.fr       */
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

static void	print_env_error(t_minishell *shell, char *cmd, int error_code,
			char *error_msg)
{
	ft_putstr_fd("./minishell: ", STDERR_FILENO);
	ft_putstr_fd(cmd, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putstr_fd(error_msg, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
	shell->exit_status = error_code;
}

static void	execute_env_command(t_minishell *shell, char **argv)
{
	char		*cmd;
	struct stat	st;
	int			arg_idx;

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
		if (access(cmd, F_OK) != 0)
			print_env_error(shell, argv[arg_idx], 127,
				"No such file or directory");
		else if (stat(cmd, &st) == 0)
		{
			if (S_ISDIR(st.st_mode))
				print_env_error(shell, argv[arg_idx], 126, "Permission denied");
			else
				print_env_error(shell, argv[arg_idx], 126, "Not a directory");
		}
		else
			print_env_error(shell, argv[arg_idx], 127,
				"No such file or directory");
	}
	else if (access(cmd, F_OK) != 0)
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

char	*get_value_env(t_minishell *mini, const char *key)
{
	t_env	*cur;

	cur = mini->env_list;
	while (cur)
	{
		if (ft_strcmp(cur->key, key) == 0)
			return (cur->value);
		cur = cur->next;
	}
	return (NULL);
}

char	*join_path(const char *dir, const char *cmd)
{
	char	*dir_slash;
	char	*full;

	if (!dir || !cmd)
		return (NULL);
	dir_slash = ft_strjoin(dir, "/");
	if (!dir_slash)
		return (NULL);
	full = ft_strjoin(dir_slash, cmd);
	free(dir_slash);
	return (full);
}
