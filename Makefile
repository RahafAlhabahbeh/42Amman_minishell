# Compiler and Flags
CC = cc
CFLAGS = -Wall -Werror -Wextra

# Directories
OBJ_DIR = obj
SRC_DIR = src
INC_DIR = include
LIBFT_DIR = libft

# Source Files
SRC = main.c pipe.c redirections.c shell.c \
signal_handling.c utils.c
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)

# Library
LIBFT = $(LIBFT_DIR)/libft.a
NAME = minishell

# Default Rule
all: $(LIBFT) $(NAME)

# Compile Minishell
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBFT) -o $(NAME) -lreadline

# Compile Object Files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Create Object Directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Build Libft
$(LIBFT):
	make -C $(LIBFT_DIR)

# Cleanup
clean:
	rm -rf $(OBJ_DIR)
	make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	make -C $(LIBFT_DIR) fclean

# Rebuild
re: fclean all

.PHONY: all clean fclean re
