/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ral-haba <ral-haba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:30 by ral-haba          #+#    #+#             */
/*   Updated: 2025/03/23 11:24:50 by ral-haba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include <signal.h>
#include <string.h>
#include <fcntl.h>  // open()
#include <stdio.h>   // For printf, perror
#include <stdlib.h>  // For malloc, free, exit
#include <unistd.h>  // For fork, execve
#include <sys/types.h>  // For pid_t
#include <sys/wait.h>   // For wait
#include <readline/readline.h>  // For readline()
#include <readline/history.h>   // For add_history()
#include "../libft/libft.h"

void redirect_output(char *filename, int append);
void redirect_input(char *filename);
void heredoc(char *delimiter);
void	child_process(int *pipe_fd, char **cmd, char **envp, int in_out);
void execute_pipe(char **cmd1, char **cmd2, char **envp);
char	*join_path(char *dir, char *cmd);
char	*get_cmd_path(char *cmd, char **paths);
void	execute_command(char *cmd, char **args, char **envp);
void redirect_output(char *filename, int append);
void	redirect_input(char *filename);
void	heredoc(char *delimiter);
void	process_redirections(char **cmd);
void	execute_with_redirections(char **cmd, char **envp);
void	shell_loop(char **envp);
void	handle_sigint(int sig);
void setup_signals(void);
void handle_eof(void);
int	open_file(char *filename, int flags, int mode);
void	free_str_array(char **arr);
void	ft_free_split_recursive(char **array, int index);

#endif
