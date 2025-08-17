/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rahaf <rahaf@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:00:00 by rahaf             #+#    #+#             */
/*   Updated: 2025/08/17 17:05:08 by rahaf            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
#include <pwd.h>
#include <stddef.h>


# include "../libft/libft.h"

extern volatile sig_atomic_t g_received_signal;
extern volatile sig_atomic_t g_child_running;
extern volatile sig_atomic_t g_in_child_process;

typedef struct s_env
{
	int		index;
	char		*key;
	char		*value;
	struct s_env	*next;
}	t_env;

typedef enum e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HERE_DOC
}	t_token_type;

typedef struct s_token
{
	char		*value;
	t_token_type	type;
	char		quote;
	struct s_token	*next;
}	t_token;

typedef struct s_heredoc
{
	char		*delimiter;
	char		quote;
	struct s_heredoc	*next;
}	t_heredoc;

typedef struct s_cmd
{
	char		**argv;
	t_token_type	in_type;
	t_token_type	out_type;
	char		*input_file_name;
	char		*output_file_name;
	char		input_quote;
	char		*heredoc_temp_file;
	int		heredoc_fd;
	t_heredoc	*heredoc_list;
	struct s_cmd	*next;
	struct s_cmd	*prev;
	int		original_stdin;
	int		original_stdout;
}	t_cmd;

typedef struct s_minishell
{
	char	*promp_input;
	t_cmd	*cmd;
	int	cmd_count;
	int	token_count;
	int	pipex_count;
	t_token	*token;
	char	buff[1024];
	char	**envp;
	t_env	*env_list;
	int	fd_out;
	int	exit_status;
}	t_minishell;

typedef struct s_pipe_data
{
	int		*pipefds;
	int		i;
	int		n;
	char	**envp;
}	t_pipe_data;

typedef struct s_exec_vars
{
	int		i;
	int		prev_fd;
	int		pipefd[2];
	pid_t	pid;
	t_cmd	*cmd;
}	t_exec_vars;

typedef struct s_tokenize_data
{
	size_t		i;
	size_t		len;
	char		buf[4096];
	int			buf_i;
	t_token		*head;
	t_token		*tail;
	char		current_quote;
	char		overall_quote;
}	t_tokenize_data;

/* tokenizer */
t_token	*tokenize(t_minishell *minishell);
int		tokenize_main_loop(t_minishell *mini, t_tokenize_data *data);
int		handle_escape_char(t_minishell *mini, size_t *i, char *buf,
			int *buf_i);
int		handle_escape_in_quotes(t_minishell *mini, size_t *i, char *buf,
			int *buf_i);
int		handle_quotes(t_minishell *mini, size_t *i, char *c,
			char *current_quote);
t_token	*new_token(const char *value, t_token_type type, char quote);
void	append_token(t_token **head, t_token **tail, t_token *tok);
int		handle_redirection(t_minishell *mini, t_tokenize_data *data, char c);
int		finalize_token(t_tokenize_data *data);
int		handle_other_escapes(t_minishell *mini, size_t *i,
			char *buf, int *buf_i, char next_char);
int		process_char(t_minishell *mini, t_tokenize_data *data);
void	init_shell(t_minishell *minishell);
void	init(t_minishell *mini);
char	*handle_continuation(t_minishell *minishell, char *full_input);
char	*handle_readline_input(t_minishell *minishell);
char	*append_continuation_line(char *full_input, char *line);
int	has_command_content(char *line, int len);
char	*handle_continuation_eof(t_minishell *ms, char *full_input);

t_token	*expand(t_minishell *minishell);
void	free_token_list(t_token *list);
t_token	*create_token(char *val, t_token_type type, char quote);
int	add_split_tokens(t_token **list, t_token **tail, char *str, t_token_type type);
int	process_token(t_minishell *mini, t_token *cur, t_token **list, t_token **tail);
int	extract_var_name(const char *str, int *pos, char *var_name, int *is_braced);
char	*safe_resize_buffer(char *buffer, size_t *capacity, size_t needed);
char	*expand_tilde(t_minishell *minishell, const char *str);
char	*replace_var(t_minishell *minishell, const char *str, char quote);
void	execute_child_command_pipe(t_minishell *ms, t_pipe_data *data);

void	init_env_list(t_minishell *mini, char **envp);
void	init_cmd(t_minishell *mini);
char	*get_value_env(t_minishell *mini, const char *key);
void	print_export_list(t_env *env_list);
void	print_env_list(t_env *env_list);
t_env	*set_env_value(t_minishell *mini, char *key, char *value);
void	unset_env(t_env **env, const char *key);
int	*init_pipes(int n);
void	close_all_pipes(int *pipefds, int n);
void	setup_child_pipes(int *pipefds, int i, int n);
void	count_pipe(t_minishell *minishell);
void	safe_pipe(int pipe_fds[2]);
int	count_args_for_command(t_token *start);
void merge_equal_sign_tokens(t_cmd *cmd);
int	init_cmd_argv(t_minishell *mini);
void	set_redirection(t_minishell *mini, int cmd_index, const char *filename, t_token_type type);
int	handle_redir(t_minishell *mini, t_token *cur, int cmd_index);
void	handle_file_redirection(t_cmd *cmd);
int	put_token_to_commands(t_minishell *minishell);

int	is_builtin(char *cmd);
void	execute_builtin(t_minishell *minishell, int i);
int	redirect_input(const char *file);
int	redirect_output(const char *file);
int	redirect_output_append(const char *file);
int	handle_redirections(t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last);


/* heredoc functions */
int	handle_heredoc(t_minishell *mini, t_cmd *cmd);
void	redirect_heredoc_input(t_cmd *cmd);
int	create_heredoc_temp_file_with_quote(t_minishell *mini,
	const char *delimiter, char **temp_filename_ptr, char quote_char);
ssize_t	read_heredoc_line(t_minishell *mini, char **line, size_t *line_size);
ssize_t	ft_getline(char **lineptr, size_t *n, int fd);
char *expand_heredoc_line(t_minishell *mini, char *line, int expand_vars);
void	cleanup_heredoc_files(t_minishell *mini);
void	handle_heredoc_sigint(int sig);
void	add_heredoc_to_list(t_cmd *cmd, const char *delimiter, char quote);
void	free_heredoc_list(t_heredoc *list);
int	create_heredoc_temp_file(t_minishell *mini, const char *delimiter, char **temp_filename_ptr, char quote_char);
int	write_heredoc_content(t_minishell *mini, int fd, const char *delimiter, int expand_vars);
int	open_heredoc_file(const char *filename);
char	*generate_temp_filename(int counter);
void	close_unused_heredoc_fds(t_minishell *mini, t_cmd *current_cmd);
void	close_all_heredoc_fds(t_minishell *mini);
void	close_unused_fds(int start_fd, int max_fd);
void	close_extra_fds(int prev_fd, int *pipe_fds, int is_last);
int	process_multiple_heredocs(t_minishell *mini, t_cmd *cmd);
void	execute_command(t_minishell *minishell, char **envp);
void	call_env(t_minishell *mini, char **argv);
void	call_export(t_minishell *mini, char **argv);
void	call_unset(t_minishell *mini, char **argv);
void	call_echo(t_minishell *mini, char **argv);
void	call_pwd(t_minishell *mini);
void	call_cd(t_minishell *mini, char **argv);
void	call_dot(t_minishell *mini, char **argv);
int	count_env_vars(t_env *env_list);
char	*get_user_home_dir(void);
char	*handle_tilde_expansion(t_minishell *mini, char *path);
char	*handle_cd_home(t_minishell *mini);
char	*handle_cd_oldpwd(t_minishell *mini);
char	*resolve_cd_path(t_minishell *mini, char **argv);

int	is_valid_identifier(const char *s);
void	export_print_sorted_env(char **arr, int count);
char	*merge_args(char **argv, int start, int *consumed);

char	*join_path(const char *dir, const char *cmd);
char	*resolve_cmd_path(char *cmd, t_minishell *mini);
int	resolve_cmd_path_with_status(char *cmd, t_minishell *mini, char **path);
int	check_direct_cmd(char *cmd, char **path);
int	find_in_paths_with_status(char **paths, char *cmd, char **path);
int	check_candidate(char *full, char *cmd, char **path, char **paths);
char	*find_in_paths(char **paths, char *cmd);
char	*check_path(char *full, char *cmd, char **paths);
int	is_executable(const char *path);
char	*check_absolute_path(char *cmd);
void	free_paths_array(char **paths);
int	is_directory(const char *path);
void	free_minishell(t_minishell *mini);
void	free_tokens(t_token *head);
void	free_cmds_array(t_cmd *cmd_array, int count);
void	free_env_list(t_env *env);
void	free_commands(t_cmd *cmds, int count);
void	free_env_array(char **arr, int count);
void	reset_minishell(t_minishell *mini);
void	cleanup_child_process(t_minishell *mini);
void	execute_piped_commands(t_minishell *minishell, char **envp);

void	handle_sigint(int sig);
void	setup_signals(void);
void	set_child_running(int running);
int	is_child_running(void);
void	set_minishell_pointer(t_minishell *mini);
int	check_sigint_received(void);
int	peek_sigint_received(void);
int	check_sigquit_received(void);
int	get_received_signal(void);
void	reset_received_signal(void);

/* Process context functions */
void	set_in_child_process(int in_child);
int	is_in_child_process(void);
void	handle_child_process(t_minishell *mini, t_cmd *cmd, char **envp);
void	execute_child_process(t_minishell *mini, t_cmd *cmd, int prev_fd, int *pipe_fds, int is_last, char **envp);
void	close_pipe_fds(int *pipe_fds);
void	process_heredocs(t_minishell *mini);
void	handle_empty_command(t_cmd *cmd);
int	execute_parent_process(int prev_fd, int *pipe_fds, int is_last);
/* Global signal variable for signal handling */
extern volatile sig_atomic_t	g_received_signal;
void	call_exit(t_minishell *mini, char **argv);
void	child_sig();

void	execute_builtin_cmd(t_minishell *mini, t_cmd *cmd);
int	is_str_in_set(const char *str, char **set);
int	ft_strcmp(const char *s1, const char *s2);

/* Debug functions */
void	print_tokens(t_token *token);
void	print_commands(t_cmd *cmd);

int	is_invalid_token(t_token_type type);
int	is_valid_syntax(t_token *tokens);

int	is_one_command(t_minishell *mini);
void	execute_one_command(t_minishell *mini, char **envp);
void	multiple_command_execution(t_minishell *mini, char **envp);
void	execute_loop(t_minishell *mini, char **envp, pid_t *pids);
pid_t	handle_command_iteration(t_minishell *mini, char **envp, t_cmd *cmd, int i);
int	should_run_builtin_in_parent(t_cmd *cmd, int index, int total_pipes);
int	is_redirection_present(t_cmd *cmd);
void	handle_child_process2(t_minishell *mini, t_cmd *cmd, int prev_fd, int *pipefd, int i, char **envp);
int	handle_empty_command2(t_minishell *mini, t_cmd *cmd);
int	handle_parent_builtin_child(t_minishell *mini, t_cmd *cmd);
int	handle_parent_builtin(t_minishell *mini, t_cmd *cmd, int prev_fd, int *pipefd, int i, pid_t *pids);
void	handle_parent_process(t_minishell *mini, pid_t pid);
void	execute_child_command(t_minishell *mini, t_cmd *cmd, int i, char **envp);

void	save_original_fds(t_cmd *cmd);
void	restore_original_fds(t_cmd *cmd);
int	is_numeric(const char *str);
int	is_valid_exit_arg(const char *str);
void	handle_invalid_exit_arg(char *arg, t_minishell *mini);
void	handle_too_many_args(t_minishell *mini);

#endif
