CC = cc
CFLAGS = -Wall -Werror -Wextra
OBJ_DIR = obj
SRC_DIR = src
INC_DIR = include
SRC = main.c shell.c utils.c redirections.c
OBJ = $(SRC:%.c=$(OBJ_DIR)/%.o)
NAME = minishell

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
