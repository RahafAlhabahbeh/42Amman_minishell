/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/14 23:47:56 by rahaf            ###   ########.fr       */
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
void free_tokens(t_token *head);
void init_shell(t_minishell *minishell);
void init(t_minishell *mini);
t_cmd *init_cmd(void);
// t_token *new_token(const char *value, t_token_type type);
// t_env *init_env(const char *key, const char *value, int index);
char	*my_getenv(char *name, char **env);
void call_env(t_minishell *minishell, char **envp);
t_token *expand(t_minishell *minishell, char **envp);
char *replace_var(const char *str, char **envp, char quote);
// char *get_env_value(const char *var, t_minishell *minishell);
t_env   *init_env_list(char **envp);
char    *get_env_value(const char *key, t_env *env);
void    set_env_value(t_env **env, const char *key, const char *value);
void    unset_env(t_env **env, const char *key);
void    free_env_list(t_env *env);


#endif
