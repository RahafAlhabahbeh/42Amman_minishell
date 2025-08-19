CC         = cc
CFLAGS     = -Wall -Werror -Wextra -g -fno-pie # -fsanitize=address -g

# Directories
OBJ_DIR    = obj
SRC_DIR   = srcs/minilib
MAIN_DIR  = srcs/main
INC_DIR    = include
LIBFT_DIR  = libft


# Source files
SRC = \
    $(SRC_DIR)/parsing/get_tokens.c  \
    $(SRC_DIR)/parsing/tokenize_loop.c  \
    $(SRC_DIR)/parsing/token_helpers.c  \
    $(SRC_DIR)/parsing/process_char.c  \
    $(SRC_DIR)/parsing/expand.c  \
    $(SRC_DIR)/parsing/expand_tilde.c  \
    $(SRC_DIR)/parsing/extract_var_name.c  \
    	$(SRC_DIR)/parsing/replace_var.c  \
	$(SRC_DIR)/parsing/expand_helpers.c  \
	$(SRC_DIR)/parsing/expand_helpers2.c  \
	$(SRC_DIR)/parsing/token_utilities.c  \
    $(SRC_DIR)/parsing/get_commands.c \
    $(SRC_DIR)/parsing/get_commands2.c \
    $(SRC_DIR)/parsing/merge_equal_sign_tokens.c \
    $(MAIN_DIR)/main.c \
    $(MAIN_DIR)/print.c \
    $(MAIN_DIR)/print_debug.c \
	$(SRC_DIR)/init/init.c \
	$(SRC_DIR)/init/init_shell.c \
	$(SRC_DIR)/init/init_env.c \
	$(SRC_DIR)/init/init_env2.c \
	$(SRC_DIR)/init/init_env3.c \
	$(SRC_DIR)/init/init_command.c \
	$(SRC_DIR)/init/continuation.c \
	$(SRC_DIR)/commands/call_env.c \
	$(SRC_DIR)/commands/call_echo.c \
	$(SRC_DIR)/commands/call_pwd.c \
	$(SRC_DIR)/commands/call_export.c \
	$(SRC_DIR)/commands/call_unset.c \
	$(SRC_DIR)/commands/call_exit.c \
	$(SRC_DIR)/commands/call_cd.c \
	$(SRC_DIR)/commands/call_cd2.c \
	$(SRC_DIR)/commands/call_dot.c \
	$(SRC_DIR)/commands/exit_utils.c \
	$(SRC_DIR)/utils/resolve_path.c \
	$(SRC_DIR)/utils/resolve_path_utils.c \
	$(SRC_DIR)/utils/resolve_path_utils2.c \
	$(SRC_DIR)/utils/export_utils.c \
	$(SRC_DIR)/utils/fd_utils.c \
	$(SRC_DIR)/pipe/pipe_exec.c \
	$(SRC_DIR)/pipe/pipe_utils.c \
	$(SRC_DIR)/pipe/redirection.c \
	$(SRC_DIR)/pipe/execute_child.c \
	$(SRC_DIR)/execution/execute_command.c \
	$(SRC_DIR)/execution/execute_builtin.c \
	$(SRC_DIR)/execution/execute_one_command.c \
	$(SRC_DIR)/execution/multiple_command_execution.c \
	$(SRC_DIR)/execution/handle_child_process.c \
	$(SRC_DIR)/execution/execute_command2.c \
	$(SRC_DIR)/execution//execute_child_command.c   \
	$(SRC_DIR)/execution//execute_loop.c      \
	$(SRC_DIR)/execution//handle_empty_command.c \
	$(SRC_DIR)/execution/convert_env_to_array.c \
	$(SRC_DIR)/redirection/redirection.c \
	$(SRC_DIR)/redirection/heredoc.c \
	$(SRC_DIR)/redirection/ft_getline.c \
	$(SRC_DIR)/redirection/create_heredoc_temp_file_with_quote.c \
	$(SRC_DIR)/redirection/heredoc_quote_utils.c \
	$(SRC_DIR)/redirection/heredoc_signal.c \
	$(SRC_DIR)/redirection/handle_heredoc.c  \
	$(SRC_DIR)/redirection/expand_heredoc.c \
	$(SRC_DIR)/free/free_minishell.c	\
	$(SRC_DIR)/free/free_tokens.c	\
	$(SRC_DIR)/free/free_cmds.c	\
	$(SRC_DIR)/free/free_utils.c	\
	$(SRC_DIR)/check_error/syntax_error.c	\
	$(SRC_DIR)/signal/signals.c \
	$(SRC_DIR)/signal/signal_utils.c \
	$(SRC_DIR)/signal/signal_utils2.c \
	$(SRC_DIR)/signal/signal_utils3.c


# Object files (mirror the SRC tree under obj/)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# The libft library and our final binary
LIBFT = $(LIBFT_DIR)/libft.a
NAME  = minishell

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline -lncurses -no-pie

# Compile each .c → .o, creating subdirs as needed
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Top-level obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Build libft automatically
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

clean:
	rm -rf $(OBJ_DIR)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all
