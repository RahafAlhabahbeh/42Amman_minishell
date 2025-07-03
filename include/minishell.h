/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 12:44:30 by ral-haba          #+#    #+#             */
/*   Updated: 2025/07/03 16:00:50 by rahaf            ###   ########.fr       */
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

typedef enum e_token_type
{
    TOK_WORD,
    TOK_PIPE,
    TOK_RED_IN,
    TOK_RED_OUT,
    TOK_HEREDOC,
    TOK_APPEND,
    TOK_EOF
} t_token_type;

typedef struct s_token
{
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

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
t_token			*lexer(char *input);
t_token_type	get_token_type(char *str);
void			token_add_back(t_token **lst, t_token *new);
void	free_tokens(t_token *head);
void	free_split(char **parts);

#endif
