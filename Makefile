# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aperez-b <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/08/29 10:50:06 by aperez-b          #+#    #+#              #
#    Updated: 2022/01/25 16:45:20 by aperez-b         ###   ########.fr        #
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

SHELL=/bin/bash
UNAME = $(shell uname -s)

# Properties for MacOS
CDEBUG = #-g3 -fsanitize=address
CHECKER = tests/checker_Mac
ifeq ($(UNAME), Linux)
	#Properties for Linux
	LEAKS = valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --trace-children=yes -s -q 
endif

# Make variables
AR = ar rcs
CFLAGS = -Wall -Wextra -Werror -MD
RM = rm -f
CC = gcc
SRC_DIR = src
SRCB_DIR = srcb
SRC_LFT_DIR = src_lft
OBJ_DIR = obj
OBJB_DIR = objb
OBJ_LFT_DIR = obj_lft
BIN_DIR = bin
BIN = pipex
NAME = $(BIN_DIR)/$(BIN)
PRINTF = LC_NUMERIC="en_US.UTF-8" printf

SRC_LFT = ft_free_matrix.c ft_lstadd_back.c ft_lstclear.c	\
		  ft_lstsize.c ft_putstr_fd.c ft_strncmp.c			\
		  ft_split.c ft_strdup.c ft_strjoin.c ft_strlen.c	\
		  ft_strnstr.c ft_lstdelone.c ft_strlcpy.c			\
		  ft_substr.c

SRC = pipex.c pipex_utils.c main.c

SRCB = pipex_bonus.c pipex_utils_bonus.c main_bonus.c

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:.c=.o))

OBJ_LFT = $(addprefix $(OBJ_LFT_DIR)/, $(SRC_LFT:.c=.o))

OBJB = $(addprefix $(OBJB_DIR)/, $(SRCB:.c=.o))

# Progress vars
SRC_COUNT_TOT := $(shell expr $(shell echo -n $(SRC) | wc -w) - $(shell ls -l $(OBJ_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRC_COUNT_TOT) -le 0; echo $$?),0)
	SRC_COUNT_TOT := $(shell echo -n $(SRC) | wc -w)
endif
SRC_COUNT := 0
SRC_PCT = $(shell expr 100 \* $(SRC_COUNT) / $(SRC_COUNT_TOT))
SRCB_COUNT_TOT := $(shell expr $(shell echo -n $(SRCB) | wc -w) - $(shell ls -l $(OBJB_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRCB_COUNT_TOT) -le 0; echo $$?),0)
	SRCB_COUNT_TOT := $(shell echo -n $(SRCB) | wc -w)
endif
SRCB_COUNT := 0
SRCB_PCT = $(shell expr 100 \* $(SRCB_COUNT) / $(SRCB_COUNT_TOT))
SRC_LFT_COUNT_TOT := $(shell expr $(shell echo -n $(SRC_LFT) | wc -w) - $(shell ls -l $(OBJ_LFT_DIR) 2>&1 | grep ".o" | wc -l) + 1)
ifeq ($(shell test $(SRC_LFT_COUNT_TOT) -le 0; echo $$?),0)
	SRC_LFT_COUNT_TOT := $(shell echo -n $(SRC_LFT) | wc -w)
endif
SRC_LFT_COUNT := 0
SRC_LFT_PCT = $(shell expr 100 \* $(SRC_LFT_COUNT) / $(SRC_LFT_COUNT_TOT))

all: $(NAME)

$(NAME): create_dirs $(OBJ_LFT) $(OBJ)
	@$(CC) $(CFLAGS) $(CDEBUG) $(OBJ) $(OBJ_LFT) -o $@
	@$(PRINTF) "\r%100s\r$(GREEN)$(BIN) is up to date!$(DEFAULT)\n"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@$(eval SRC_COUNT = $(shell expr $(SRC_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_COUNT) $(SRC_COUNT_TOT) $(SRC_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) -c $< -o $@

$(OBJ_LFT_DIR)/%.o: $(SRC_LFT_DIR)/%.c
	@$(eval SRC_LFT_COUNT = $(shell expr $(SRC_LFT_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRC_LFT_COUNT) $(SRC_LFT_COUNT_TOT) $(SRC_LFT_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) -c $< -o $@

bonus: create_dirs $(OBJ_LFT) $(OBJB)
	@$(CC) $(CFLAGS) $(CDEBUG) $(OBJB) $(OBJ_LFT) -o $(NAME)

$(OBJB_DIR)/%.o: $(SRCB_DIR)/%.c
	@$(eval SRCB_COUNT = $(shell expr $(SRCB_COUNT) + 1))
	@$(PRINTF) "\r%100s\r[ %d/%d (%d%%) ] Compiling $(BLUE)$<$(DEFAULT)..." "" $(SRCB_COUNT) $(SRCB_COUNT_TOT) $(SRCB_PCT)
	@$(CC) $(CFLAGS) $(CDEBUG) -c $< -o $@

create_dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_LFT_DIR)
	@mkdir -p $(OBJB_DIR)
	@mkdir -p $(BIN_DIR)

test: bonus
	@$(PRINTF) "$(YELLOW)Performing test with custom parameters...$(DEFAULT)\n\n"
	@$(PRINTF) "Command: $(GRAY)$(LEAKS)./$(NAME) $(N)$(DEFAULT)\n\n"
	@$(LEAKS)./$(NAME) $(N)

clean:
	@$(PRINTF) "$(CYAN)Cleaning up object files in $(OBJ_DIR), $(OBJB_DIR) and $(OBJ_LFT_DIR)...$(DEFAULT)\n"
	@$(RM) -r $(OBJ_DIR)
	@$(RM) -r $(OBJB_DIR)
	@$(RM) -r $(OBJ_LFT_DIR)

fclean: clean
	@$(RM) -r $(BIN_DIR)
	@$(PRINTF) "$(CYAN)Removed $(BIN)$(DEFAULT)\n"

norminette:
	@$(PRINTF) "$(CYAN)\nChecking norm for $(BIN)...$(DEFAULT)\n"
	@norminette -R CheckForbiddenSourceHeader $(SRC_DIR) $(SRCB_DIR) $(SRC_LFT_DIR) inc/

re: fclean
	@make all

git:
	git add .
	git commit
	git push

-include $(OBJ_DIR)/*.d
-include $(OBJB_DIR)/*.d
-include $(OBJB_LFT_DIR)/*.d

.PHONY: all clean fclean norminette create_dirs test git re
