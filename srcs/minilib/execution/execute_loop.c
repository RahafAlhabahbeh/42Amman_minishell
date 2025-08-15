/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/12 00:00:00 by dal-mahr          #+#    #+#             */
/*   Updated: 2025/08/15 17:53:42 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

void	execute_loop(t_minishell *mini, char **envp, pid_t *pids)
{
	int		i;
	int		pipefd[2];
	int		prev_fd;
	pid_t	pid;
	t_cmd	*cmd;
	struct sigaction	sa;

	prev_fd = -1;
	process_heredocs(mini);
	i = 0;
	while (i <= mini->pipex_count)
	{
		pipefd[0] = -1;
		pipefd[1] = -1;
		
		if (i < mini->pipex_count && pipe(pipefd) == -1)
		{
			if (prev_fd != -1)
				close(prev_fd);
			perror("pipe");
			mini->exit_status = 1;
			return;
		}
		sa.sa_handler = SIG_IGN;
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = 0;
		sigaction(SIGINT, &sa, NULL);
		cmd = &mini->cmd[i];
		if (handle_parent_builtin(mini, cmd, prev_fd, pipefd, i, pids))
		{
			prev_fd = execute_parent_process(prev_fd, pipefd,
					i == mini->pipex_count);
				
			i++;
			continue ;
		}
		pid = fork();
		if (pid == -1)
		{
			close_pipe_fds(pipefd);
			if (prev_fd != -1)
				close(prev_fd);
			set_child_running(0);
			perror("fork");
			mini->exit_status = 1;
			return;
		}
		else if (pid == 0)
			handle_child_process2(mini, cmd, prev_fd, pipefd, i, envp);
		else
		{
			pids[i] = pid;
			prev_fd = execute_parent_process(prev_fd, pipefd,
					i == mini->pipex_count);
		}
		i++;
	}
	if (prev_fd != -1)
		close(prev_fd);
	close_all_heredoc_fds(mini);
}
