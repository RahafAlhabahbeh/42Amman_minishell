/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_debug.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/12 17:30:00 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_argv_list(char **argv)
{
	int	j;

	j = 0;
	while (argv[j])
	{
		printf("    argv[%d]: [%s]\n", j, argv[j]);
		j++;
	}
}

static void	print_input_type(t_cmd *cmd)
{
	if (cmd->in_type == REDIR_IN)
		printf("    input: REDIR_IN <%s>\n", cmd->input_file_name);
	else if (cmd->in_type == HERE_DOC)
		printf("    input: HERE_DOC\n");
	else if (cmd->in_type == PIPE)
		printf("    input: PIPE\n");
	else
		printf("    input: none\n");
}

static void	print_output_type(t_cmd *cmd)
{
	if (cmd->out_type == REDIR_OUT)
		printf("    output: REDIR_OUT >%s>\n", cmd->output_file_name);
	else if (cmd->out_type == REDIR_APPEND)
		printf("    output: APPEND >>%s>\n", cmd->output_file_name);
	else if (cmd->out_type == PIPE)
		printf("    output: PIPE\n");
	else
		printf("    output: none\n");
}

void	print_commands(t_cmd *cmd)
{
	int	i;

	i = 0;
	printf("\n🚀 Command List:\n");
	while (cmd)
	{
		printf("  Cmd[%d]:\n", i);
		if (cmd->argv)
			print_argv_list(cmd->argv);
		else
			printf("    (no argv)\n");
		print_input_type(cmd);
		print_output_type(cmd);
		cmd = cmd->next;
		i++;
	}
	printf("🔚 End of Commands\n");
}
