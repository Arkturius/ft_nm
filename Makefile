# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rgramati <rgramati@student.42angouleme.fr  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/05 17:09:56 by rgramati          #+#    #+#              #
#    Updated: 2025/04/19 18:18:57 by rgramati         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		=	$(notdir $(shell pwd))

SRC_DIR		:=	src
INC_DIR		:=	include
LIB_DIR		:=	lib
OBJ_DIR		:=	build

include			sources.mk
SRCS		:=	$(addprefix $(SRC_DIR)/, $(SRCS))

LIB_DIRS	:=	$(addprefix $(LIB_DIR)/,$(LIBS))

COPTS_DIRS	:=	$(INC_DIR)
COPTS		+=	$(foreach dir, $(COPTS_DIRS),-I$(dir))

OBJS 		:=	$(addprefix $(OBJ_DIR)/, $(SRCS:%.c=%.o))

CC			:=	clang

CFLAGS		:=	-Wall -Wextra -MMD -MP -g

ifeq ($(TEMP), 1)
	CFLAGS	+=	--save-temps=obj
endif

VERBOSE		?=	0
ifeq ($(VERBOSE), 1)
	CFLAGS	+=	-DVERBOSE=1
endif

RM			:=	rm -rf
MKDIR		:=	mkdir -p

MAKE		+=	--no-print-directory

#
# Rules
#

all:					$(NAME)

$(NAME):				$(OBJS)
	@echo " $(GREEN)$(BOLD)$(ITALIC)■$(RESET)  linking	$(GREEN)$(BOLD)$(ITALIC)$(NAME)$(RESET)"
	@$(CC) $(CFLAGS) $(COPTS) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/src/%.o: 	src/%.c
	@$(MKDIR) $(@D)
	@echo " $(CYAN)$(BOLD)$(ITALIC)■$(RESET)  compiling	$(GRAY)$(BOLD)$(ITALIC)$(notdir $@) from $(GRAY)$(BOLD)$(ITALIC)$(notdir $^)$(RESET)"
	@$(CC) $(CFLAGS) -o $@ -c $(COPTS) $<

clean:
	@if [ -d $(OBJ_DIR) ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(NAME)/$(OBJ_DIR)$(RESET)"; \
		$(RM) $(OBJ_DIR); \
	fi

fclean:					clean
	@if [ -f "$(NAME)" ]; then \
		echo " $(RED)$(BOLD)$(ITALIC)■$(RESET)  deleted	$(RED)$(BOLD)$(ITALIC)$(NAME)$(RESET)"; \
		$(RM) $(NAME); \
	fi;
	@for DIR in $(LIB_DIRS); \
	do $(MAKE) -C $$DIR fclean; \
	done;

re:					fclean all

-include	$(OBJS:.o=.d)

.PHONY:		all clean fclean re
.SILENT:	all clean fclean re

#
# COLORS
# 

BOLD			=	\033[1m
ITALIC			=	\033[3m

RED				=	\033[31m
GREEN			=	\033[32m
YELLOW			=	\033[33m
CYAN			=	\033[36m
GRAY			=	\033[90m

RESET			=	\033[0m

LINE_CLR		=	\33[2K\r


