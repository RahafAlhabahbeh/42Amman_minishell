/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 08:55:21 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "pwd") || !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset") || !ft_strcmp(cmd, "exit")
		|| !ft_strcmp(cmd, "env"))
		return (1);
	return (0);
}

void	execute_builtin(t_minishell *minishell, int i)
{
	char	*name;

	if (i < 0 || i > minishell->pipex_count || !minishell->cmd[i].argv
		|| !minishell->cmd[i].argv[0])
		return ;
	name = minishell->cmd[i].argv[0];

	if (!ft_strcmp(name, "echo"))
		call_echo(minishell, minishell->cmd[i].argv);
	else if (!ft_strcmp(name, "pwd"))
		call_pwd(minishell);
	else if (!ft_strcmp(name, "export"))
		call_export(minishell, minishell->cmd[i].argv);
	else if (!ft_strcmp(name, "unset"))
		call_unset(minishell, minishell->cmd[i].argv);
	else if (!ft_strcmp(name, "exit"))
		call_exit(minishell, minishell->cmd[i].argv);
	else if (!ft_strcmp(name, "cd"))
		call_cd(minishell, minishell->cmd[i].argv);
	else if (!ft_strcmp(name, "env"))
		call_env(minishell, minishell->cmd[i].argv);
}

void	execute_builtin_cmd(t_minishell *mini, t_cmd *cmd)
{
	char	*name;

	if (!cmd || !cmd->argv || !cmd->argv[0])
		return ;
	name = cmd->argv[0];

	if (!ft_strcmp(name, "echo"))
		call_echo(mini, cmd->argv);
	else if (!ft_strcmp(name, "pwd"))
		call_pwd(mini);
	else if (!ft_strcmp(name, "export"))
		call_export(mini, cmd->argv);
	else if (!ft_strcmp(name, "unset"))
		call_unset(mini, cmd->argv);
	else if (!ft_strcmp(name, "exit"))
		call_exit(mini, cmd->argv);
	else if (!ft_strcmp(name, "cd"))
		call_cd(mini, cmd->argv);
	else if (!ft_strcmp(name, "env"))
		call_env(mini, cmd->argv);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}