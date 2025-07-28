/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dal-mahr <dal-mahr@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/07/27 14:16:31 by dal-mahr         ###   ########.fr       */
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

/*
// Delay between characters (500 microseconds = 0.5ms)
void print_slowly(const char *line)
{
    int i;
    i = 0;
    while (line[i])
    {
        write(1, &line[i], 1);
        usleep(500);
        i++;
    }
}
void print_banner(void)
{
    const char *banner[] = {
        "\n",
        "\033[1;35m",
        "███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗     \n",
        "████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║     \n",
        "██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║     \n",
        "██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║     \n",
        "██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗\n",
        "╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝\n",
        "\033[0m",
        "\n",
        NULL
    };
    int i = 0;
    while (banner[i])
    {
        print_slowly(banner[i]);
        usleep(80000); // 80ms between each line
        i++;
    }
}
*/

/*
typedef enum e_type
{
    INPUT_NONE,
    INPUT_FILE,
    INPUT_PIPE,
    INUPT_WORD,
    INPUT_END, //? May delete
    INPUT_HEREDOC,
    OUTPUT_NONE,
    OUTPUT_FILE,
    OUTPUT_APPEND,
    OUTPUT_PIPE
} t_type;
*/

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
    // char **input_files;
    // char **output_files;
    struct s_cmd *next;  // Next command in pipe sequence
    struct s_cmd *prev;  // Prev command in pipe sequence
    // t_env *env;
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
void redirect_input(const char *file);
void redirect_output(const char *file);
void redirect_output_append(const char *file);
void handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last);
void execute_command(t_minishell *minishell, char **envp);
void call_env(t_minishell *mini);
void call_export(t_minishell *mini, char **argv);
void call_unset(t_minishell *mini, char **argv);
void call_echo(char **argv);
void call_pwd();
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


void execute_builtin_cmd(t_cmd *cmd);        // <-- Add this
int is_str_in_set(char *s, char *set[]);     // <-- Add this
void execute_builtin(t_minishell *minishell, int i);
int ft_strcmp(const char *s1, const char *s2);



// NO
void print_tokens(t_token *token);
void print_commands(t_cmd *cmd);


int is_invalid_token(t_token_type type);
int	is_valid_syntax(t_token *tokens);


#endif
