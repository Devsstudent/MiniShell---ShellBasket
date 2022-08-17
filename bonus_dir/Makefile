FLAG = -Wall -g -Wextra -MMD -Werror
CC = gcc
LIB = ../libft
HEAD = ./includes
OBJ = $(addprefix obj/, new_main_2.o \
			garbage_collector.o \
			copy_env.o \
			lst_utils.o \
			lexing_bonus.o \
			line_lst_utils.o \
			free_exit.o \
			singleton.o \
			check_symbol.o \
			tokenization_bonus.o \
			signal_handling.o \
			expand.o \
			abstract_syntax_tree_bonus.o \
			remove_quote_before_exec.o)
D_LST = $(addprefix obj/, new_main_2.d \
			garbage_collector.d \
			copy_env.d \
			lst_utils.d \
			lexing_bonus.d \
			line_lst_utils.d \
			free_exit.d \
			singleton.d \
			check_symbol.d \
			tokenization_bonus.d \
			signal_handling.d \
			expand.d \
			abstract_syntax_tree_bonus.d \
			remove_quote_before_exec.d \
			null.d)
NAME = minishell

all : $(NAME)

$(NAME): $(OBJ)
	make bonus -s -C $(LIB)
	$(CC) $(FLAG) $(OBJ) -lreadline -L $(LIB) -lft -o $(NAME)

obj/%.o: src/%.c | object
	$(CC) $(FLAG) -I $(LIB) -I $(HEAD) -I /usr/local/opt/readline/include -c $< -o $@

object:
	@mkdir -p obj

clean :
	make -s clean -C $(LIB)
	rm -f $(NAME)

fclean :
	make -s fclean -C $(LIB)
	rm -f $(NAME)
	rm -rf obj

re : fclean all

.PHONY: all re clean fclean 
-include $(D_LST)
