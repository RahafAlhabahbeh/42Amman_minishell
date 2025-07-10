/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/02 22:14:59 by aaljazza          #+#    #+#             */
/*   Updated: 2025/07/03 00:35:30 by aaljazza        ####  ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ANTSHELL_H
#define ANTSHELL_H

#include "../libft/includes/libft.h"
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>

#define PROMPT "\033[33mantshell\033[32m$ \033[0m"

extern char **environ;

typedef struct s_minishell {
	char *input;
	char **cmd;
	char ***cmdList;
	int i;
	int j;
	int count;
	char **tok;
	int fd_in;
	int fd_out;
	int fd_app;
	char buff[1024];
}	t_minishell;

//! New Struct asmaa
// //Todo need to init
// typedef struct s_cmd_node
// {
// 	char **cmd;
// 	struct s_cmd *next;
	
// } t_cmd_node;

// t_antshell {
// 	char *input;
// 	// char **cmd;
// 	// char ***cmdList;
// 	t_cmd_node	*cmd;
// 	int i;
// 	int j;
// 	int count;
// 	char **tok;
// 	int fd_in;
// 	int fd_out;
// 	int fd_app;
// 	char buff[1024];
// };
//! End

//* #### Initialize some of elements in the antshell structure. ####
//- 	integer values
//- 	tokens array
void init(t_minishell *minishell);
//
//* #### Display prompt, take an input, and initialize other structure elements.
//- It is also count the number of tokens
//- exit if error occured
void init_shell(t_minishell *minishell);
//
//* #### loop over tokens array to check for redirections ###
void redirection(t_minishell *minishell);
//
void redir_compare1(t_minishell *minishell);
void redir_compare2(t_minishell *minishell);
void child_re(t_minishell *minishell);
void parent_re(t_minishell *minishell);
void call_echo(t_minishell *minishell, int op);
void call_pwd(t_minishell *minishell);
void call_env(t_minishell *minishell);
//
// - write an error message
// - free minishell
// - exit with specific status
void ft_exit(t_minishell *minishell, char *str, int status);
//
void free_2d(char **arr);
void    redir_op1 (char **tokens, int *k, const char *input, int *i);
void	redir_op2(char **tokens, int *k, const char *input, int *i);
void quoted(char **tokens, int *k, const char *input, int *i);
void normal_string(char **tokens, int *k, const char *input, int *i);
//
//* #### Do the first fork in the program
void main_fork (t_minishell *minishell,int pid);
//
void compare_commands (t_minishell *minishell);
//
//* #### 1. free all allocated memore 
// - Commands array
// - Tokens array
// - Input String
//* #### 2. Exit from the program if the input is [ exit ]
void check_to_free (t_minishell *minishell);
char **get_tokens (const char *input);


#endif
