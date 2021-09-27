# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/29 10:50:06 by aperez-b          #+#    #+#              #
#    Updated: 2021/09/27 18:38:14 by aperez-b         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Color Aliases
DEFAULT = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

UNAME = $(shell uname -s)

# Properties for MacOS
ECHO = echo
CDEBUG = #-g3 -fsanitize=address
CHECKER = tests/checker_Mac
ifeq ($(UNAME), Linux)
	#Properties for Linux
	ECHO = echo -e
	LEAKS = valgrind --leak-check=full --show-leak-kinds=all -s -q 
endif

# Make variables
AR = ar rcs
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
CC = gcc -MD
SRC_DIR = src
SRCB_DIR = src_bonus
OBJ_DIR = obj
OBJB_DIR = objb
BIN_DIR = bin
BIN = pipex
NAME = $(BIN_DIR)/$(BIN)
LIBFT = libft/bin/libft.a

SRC = pipex.c pipex_utils.c main.c

SRCB = 

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

OBJB = $(addprefix $(OBJB_DIR)/, $(SRCB:.c=.o))

all: $(NAME)

$(NAME): create_dirs compile_libft $(OBJ) $(OBJB)
	@$(CC) $(CFLAGS) $(CDEBUG) $(OBJ) $(OBJB) $(LIBFT) -o $@
	@$(ECHO) "$(GREEN)$(BIN) is up to date!$(DEFAULT)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(ECHO) "Compiling $(BLUE)$<$(DEFAULT)..."
	@$(CC) $(CFLAGS) $(CDEBUG) -c $< -o $@

bonus: all

compile_libft:
	@make all -C libft/

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJB_DIR)
	@mkdir -p $(BIN_DIR)

test: all
	@$(ECHO) "$(YELLOW)Performing test with custom parameters...$(DEFAULT)\n"
	@$(ECHO) "Command: $(GRAY)$(LEAKS)./$(NAME) $(N)$(DEFAULT)\n"
	@$(LEAKS)./$(NAME) $(N)

clean:
	@$(ECHO) "$(CYAN)Cleaning up object files in $(NAME)...$(DEFAULT)"
	@make clean -C libft
	@$(RM) -r $(OBJ_DIR)

fclean: clean
	@$(RM) -r $(BIN_DIR)
	@$(RM) $(LIBFT)
	@$(RM) $(NAME)
	@$(ECHO) "$(CYAN)Removed $(NAME)$(DEFAULT)"
	@$(ECHO) "$(CYAN)Removed $(LIBFT)$(DEFAULT)"

norminette:
	@$(ECHO) "$(CYAN)\nChecking norm for $(NAME)...$(DEFAULT)"
	@norminette -R CheckForbiddenSourceHeader $(SRC_M) $(SRC_B) inc/
	@make norminette -C libft/

re: fclean all
	@$(ECHO) "$(YELLOW)Cleaned and Rebuilt Everything for $(BIN)!$(DEFAULT)"

git:
	git add .
	git commit
	git push

-include $(OBJ_DIR)/*.d
-include $(OBJB_DIR)/*.d

.PHONY: all clean fclean bonus compile_libft norminette create_dirs test git re
