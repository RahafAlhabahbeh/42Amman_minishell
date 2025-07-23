CC         = cc
CFLAGS     = -Wall -Werror -Wextra

# Directories
OBJ_DIR    = obj
SRC_DIR   = srcs/minilib
MAIN_DIR  = srcs/main
INC_DIR    = include
LIBFT_DIR  = libft


# Source files
SRC = \
    $(SRC_DIR)/parsing/get_tokens.c  \
    $(SRC_DIR)/parsing/expand.c  \
    $(SRC_DIR)/parsing/get_commands.c \
    $(MAIN_DIR)/main.c \
	$(SRC_DIR)/init/init_shell.c \
	$(SRC_DIR)/init/init.c \
	$(SRC_DIR)/commands/call_env.c \
	$(SRC_DIR)/commands/call_echo.c \
	$(SRC_DIR)/commands/call_pwd.c \
	$(SRC_DIR)/commands/call_export.c \
	$(SRC_DIR)/commands/call_unset.c \
	$(SRC_DIR)/init/init_env.c \
	$(SRC_DIR)/pipe/pipe.c \
	$(SRC_DIR)/execution/execute_command.c \
	$(SRC_DIR)/redirection/redirection.c \
	$(SRC_DIR)/free/free_minishell.c	\
	$(SRC_DIR)/signal/signals.c


# Object files (mirror the SRC tree under obj/)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

# The libft library and our final binary
LIBFT = $(LIBFT_DIR)/libft.a
NAME  = minishell

.PHONY: all clean fclean re

all: $(LIBFT) $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline -lncurses

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
