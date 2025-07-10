# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aaljazza <aaljazza@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/02 22:14:48 by aaljazza          #+#    #+#              #
#    Updated: 2025/07/09 07:27:53 by aaljazza         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color
RESET	=	\033[0m
RED		=	\033[31m
GREEN	=	\033[32m
YELLOW	=	\033[33m
BLUE	=	\033[34m


# Standard
NAME	= minishell

# Compiler and Flags
CC		= 	cc
RM 		= 	rm -f
CFLAGS 	= 	-Wall -Wextra -Werror -g -Iinclude

# Directories
SRCS_DIR				=	srcs/
OBJS_DIR				=	objs/
LIBFT					=	./libft/libft.a
MAIN_DIR				=	$(SRCS_DIR)main/
MINILIB_DIR				=	$(SRCS_DIR)minilib/
INIT_DIR				=	$(MINILIB_DIR)init/
FREE_DIR				=	$(MINILIB_DIR)free/
PARS_DIR				=	$(MINILIB_DIR)parsing/
COMMANDS_DIR			=	$(MINILIB_DIR)commands/
BUILTIN_COMM_DIR		=	$(COMMANDS_DIR)builtin/
EXTERNAL_COMM_DIR		=	$(COMMANDS_DIR)external/
REDIR_DIR				=	$(MINILIB_DIR)redirections/

# Source Files

#	src/
#	|-second/
#	|-third/

INIT_SRC	=	$(INIT_DIR)init.c\
				$(INIT_DIR)init_shell.c\

FREE_SRC	=	$(FREE_DIR)ft_exit.c\
				$(FREE_DIR)free_2d.c\
				$(FREE_DIR)check_to_free.c\

PARS_SRC	= 	$(PARS_DIR)get_tokens.c\
				$(PARS_DIR)redir_op1.c\
				$(PARS_DIR)redir_op2.c\
				$(PARS_DIR)normal_string.c\
				$(PARS_DIR)quoted.c\


BULTIN_COMM_SRC	=	$(BUILTIN_COMM_DIR)commands.c\
					$(BUILTIN_COMM_DIR)call_pwd.c\
					$(BUILTIN_COMM_DIR)call_echo.c\
					$(BUILTIN_COMM_DIR)compare_commands.c

EXTERNAL_COMM_SRC	= 

COMMANDS_SRC	=	$(BULTIN_COMM_SRC) $(EXTERNAL_COMM_SRC)

REDIR_SRC		=	$(REDIR_DIR)child_re.c\
					$(REDIR_DIR)parent_re.c\
					$(REDIR_DIR)redirection.c\
					$(REDIR_DIR)redir_compare1.c\
					$(REDIR_DIR)redir_compare2.c\

MINILIB_SRC	= 	$(PARS_SRC)\
				$(INIT_SRC)\
				$(FREE_SRC)\
				$(REDIR_SRC)\
				$(COMMANDS_SRC)\

MAIN_SRC = $(MAIN_DIR)main.c\
		   $(MAIN_DIR)main_fork.c\
		#    $(MAIN_DIR)compare_input.c\

# PARS_DIR	= 	$(PARS)parse_and_execute.c


SRCS	= $(MAIN_SRC) $(MINILIB_SRC)

OBJS	= $(patsubst $(SRCS_DIR)%.c,$(OBJS_DIR)%.o,$(SRCS))


# Build Rules
# start:
# 	make all

all: $(NAME) $(LIBFT)

$(LIBFT):
	@make -s -C ./libft > /dev/null

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(CFLAGS) $(INC) $(OBJS) $(LIBFT) -o $(NAME) -lreadline > /dev/null
	@echo "\n[ $(YELLOW)$(NAME) $(RESET)] $(GREEN)has been created!$(RESET)\n"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c
#   @echo "Creating $(YELLOW)$(@D)$(RESET)/ $(GREEN)$(@F)$(RESET)"
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INC) -c $< -o $@ > /dev/null

clean:
	@$(RM) -r $(OBJS_DIR)
	@make clean -s -C libft
	@echo "\n$(BLUE)Obj files cleaned.$(RESET)\n"

fclean: clean
	@$(RM) $(NAME)
	@make fclean -s -C libft
	@echo "\n$(BLUE)Binary files cleaned.$(RESET)\n"

re: fclean all

.PHONY: all clean fclean re
