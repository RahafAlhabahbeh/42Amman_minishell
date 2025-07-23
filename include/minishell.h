/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/21 12:53:32 by dal-mahr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>

#include "../libft/libft.h"

typedef struct s_env
{
    int index;
    char *key;
    char *value;
    struct s_env *next;
}   t_env;


typedef enum e_token_type
{
    WORD,
    PIPE,
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    HERE_DOC
}               t_token_type;

typedef struct s_token
{
    char *value;
    t_token_type type;
    char quote;
    struct s_token *next;
} t_token;


typedef struct s_cmd {
    char **argv;           // ["cat"]
    t_token_type in_type;     // NONE / REDIR_IN / HEREDOC / PIPE_IN
    t_token_type out_type;    // NONE / REDIR_OUT / APPEND / PIPE_OUT
    char *input_file_name;
    char *output_file_name;
    struct s_cmd *next;  // Next command in pipe sequence
    struct s_cmd *prev;  // Prev command in pipe sequence
    // t_env *env;
} t_cmd;

typedef struct s_minishell
{
    char *promp_input;
    t_cmd *cmd;
    int token_count;
    int pipex_count;
    t_token *token;
    char buff[1024];
    char **envp;
    t_env *env_list;
    int fd_out;
}   t_minishell;

/* tokenizer */
t_token *tokenize(t_minishell *minishell);
void init_shell(t_minishell *minishell);
void init(t_minishell *mini);
t_cmd *init_cmd(void);
t_token *expand(t_minishell *minishell);
char *replace_var(t_minishell *minishell, const char *str, char quote);
void init_env_list(t_minishell *mini, char **envp);
char *get_value_env(t_minishell *mini, const char *key);
void print_export_list(t_env *env_list);
void print_env_list(t_env *env_list);
// void set_env_value(t_minishell *mini, const char *key, const char *value);
t_env *set_env_value(t_minishell *mini, char *key, char *value);
void    unset_env(t_env **env, const char *key);
void    free_env_list(t_env *env);

void count_pipe(t_minishell *minishell);
int count_args_for_command(t_token *start);
void put_token_to_commands(t_minishell *minishell);

int is_builtin(char *cmd);
void execute_builtin(t_minishell *minishell, int i);
void redirect_input(const char *file);
void redirect_output(const char *file);
void execute_command(t_minishell *minishell, char **envp);
void call_env(t_minishell *mini);
void call_export(t_minishell *mini, char **argv);
void call_unset(t_minishell *mini, char **argv);
void call_echo(char **argv);
void call_pwd();
int is_valid_identifier(const char *s);

char *join_path(const char *dir, const char *cmd);
char *resolve_cmd_path(char *cmd, char **envp);
void free_minishell(t_minishell *mini);
void free_tokens(t_token *head);
void free_cmds_array(t_cmd *cmd_array, int count);
void free_commands(t_minishell *minishell);
void execute_piped_commands(t_minishell *minishell, char **envp);

void handle_sigint(int sig);
void setup_signals(void);



#endif
