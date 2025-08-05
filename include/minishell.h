/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/05 04:37:17 by dal-mahr         ###   ########.fr       */
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
    char input_quote;         // Quote character for input redirection (for heredoc)
    char *heredoc_temp_file;  // Temporary file name for heredoc cleanup
    // char **input_files;
    // char **output_files;
    struct s_cmd *next;  // Next command in pipe sequence
    struct s_cmd *prev;  // Prev command in pipe sequence
    // t_env *env;
    int original_stdin;
    int original_stdout;
} t_cmd;

typedef struct s_minishell
{
    char *promp_input;
    t_cmd *cmd;
    int cmd_count;
    int token_count;
    int pipex_count;
    t_token *token;
    char buff[1024];
    char **envp;
    t_env *env_list;
    int fd_out;
    int exit_status;
}   t_minishell;

/* tokenizer */
t_token *tokenize(t_minishell *minishell);
void init_shell(t_minishell *minishell);
void init(t_minishell *mini);
void init_env_list(t_minishell *mini, char **envp);
// t_cmd *init_cmd(void);
void init_cmd(t_minishell *mini);
char *get_value_env(t_minishell *mini, const char *key);
void print_export_list(t_env *env_list);
void print_env_list(t_env *env_list);
// void set_env_value(t_minishell *mini, const char *key, const char *value);
t_env *set_env_value(t_minishell *mini, char *key, char *value);
void    unset_env(t_env **env, const char *key);

void count_pipe(t_minishell *minishell);
void safe_pipe(int pipe_fds[2]);
int count_args_for_command(t_token *start);
void put_token_to_commands(t_minishell *minishell);

int is_builtin(char *cmd);
void execute_builtin(t_minishell *minishell, int i);
int redirect_input(const char *file);
int redirect_output(const char *file);
int redirect_output_append(const char *file);
int handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last);

/* heredoc functions */
int handle_heredoc(t_minishell *mini, t_cmd *cmd);
void redirect_heredoc_input(const char *fd_str);
void cleanup_heredoc_files(t_minishell *mini);
void handle_heredoc_sigint(int sig);
void execute_command(t_minishell *minishell, char **envp);
void call_env(t_minishell *mini, char **argv);
void call_export(t_minishell *mini, char **argv);
void call_unset(t_minishell *mini, char **argv);
void call_echo(t_minishell *mini, char **argv);
void call_pwd(t_minishell *mini);
void call_cd(t_minishell *mini, char **argv);
int is_valid_identifier(const char *s);

char *join_path(const char *dir, const char *cmd);
char *resolve_cmd_path(char *cmd, char **envp);
void free_minishell(t_minishell *mini);
void free_tokens(t_token *head);
void free_cmds_array(t_cmd *cmd_array, int count);
void free_env_list(t_env *env);
void free_commands(t_cmd *cmds, int count);
void reset_minishell(t_minishell *mini);
void execute_piped_commands(t_minishell *minishell, char **envp);

void handle_sigint(int sig);
void setup_signals(void);
t_token *expand(t_minishell *minishell);
void	call_exit(t_minishell *mini, char **argv);


// void execute_builtin_cmd(t_cmd *cmd);        // <-- Add this
void execute_builtin_cmd(t_minishell *mini, t_cmd *cmd);
int is_str_in_set(char *s, char *set[]);     // <-- Add this
void execute_builtin(t_minishell *minishell, int i);
int ft_strcmp(const char *s1, const char *s2);



// NO
void print_tokens(t_token *token);
void print_commands(t_cmd *cmd);


int is_invalid_token(t_token_type type);
int	is_valid_syntax(t_token *tokens);

int is_one_command(t_minishell *mini);
void execute_one_command(t_minishell *mini, char **envp);
void multiple_command_execution(t_minishell *mini, char **envp);
void execute_loop(t_minishell *mini, char **envp, pid_t *pids);
pid_t handle_command_iteration(t_minishell *mini, char **envp,
                                      t_cmd *cmd, int i, int prev_fd, int *pipe_fds);
int execute_parent_process(int prev_fd, int *pipe_fds, int is_last);
void execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd,
                                  int *pipe_fds, int is_last, char **envp);
int should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes);
int is_redirection_present(t_cmd *cmd);
void save_original_fds(t_cmd *cmd);
void restore_original_fds(t_cmd *cmd);


#endif
