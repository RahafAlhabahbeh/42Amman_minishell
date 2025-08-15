/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   call_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:54:23 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 16:47:47 by rahaf            ###   ########.fr       */
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
	char	*path;
	int		status;

	status = resolve_cmd_path_with_status(argv[1], shell, &path);
	if (status != 0)
	{
		ft_putstr_fd("env: '", STDERR_FILENO);
		ft_putstr_fd(argv[1], STDERR_FILENO);
		ft_putstr_fd("': No such file or directory\n", STDERR_FILENO);
		shell->exit_status = 127;
		return ;
	}
	
	// Command exists - for minishell we just succeed without execution
	// Full env command execution would require complex fork/exec handling
	free(path);
	shell->exit_status = 0;
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
