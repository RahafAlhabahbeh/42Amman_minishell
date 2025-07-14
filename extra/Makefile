# Colors
RESET	=	\033[0m
RED		=	\033[31m
GREEN	=	\033[32m
YELLOW	=	\033[33m
BLUE	=	\033[34m

# Compiler and flags
CC		= cc
RM		= rm -f
#CFLAGS	= -Wall -Wextra -Werror -g -Iinclude
CFLAGS	= -g -Iinclude
# Libft static library
LIBFT	= ../libft/libft.a

# Source files for each program
SRC1	= pipe1.c
SRC2	= pipe2.c

# Output binaries
NAME1	= pipe1
NAME2	= pipe2

# Build rules
all: $(NAME1) $(NAME2)

$(NAME1): $(SRC1) $(LIBFT)
	@$(CC) $(CFLAGS) $< $(LIBFT) -o $@
	@echo "$(GREEN)Compiled $(NAME1)$(RESET)"

$(NAME2): $(SRC2) $(LIBFT)
	@$(CC) $(CFLAGS) $< $(LIBFT) -o $@
	@echo "$(GREEN)Compiled $(NAME2)$(RESET)"

# Clean object files if any
clean:
	@echo "$(YELLOW)Nothing to clean (no object files)$(RESET)"

# Clean binaries
fclean: clean
	@$(RM) $(NAME1) $(NAME2)
	@echo "$(RED)Removed binaries$(RESET)"

re: fclean all

.PHONY: all clean fclean re

