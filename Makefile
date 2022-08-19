FLAG = -Wall -g -Wextra -MMD -Werror
LIB = ./libft
HEAD = ./includes
OBJ = $(addprefix obj/, main.o \
			garbage_collector.o \
			copy_env.o \
			lst_utils.o \
			lexing.o \
			line_lst_utils.o \
			free_exit.o \
			singleton.o \
			check_symbol.o \
			tokenization.o \
			signal_handling.o \
			expand.o \
			abstract_syntax_tree.o \
			error.o \
			remove_quote_before_exec.o)
D_LST = $(addprefix obj/, main.d \
			garbage_collector.d \
			copy_env.d \
			lst_utils.d \
			lexing.d \
			line_lst_utils.d \
			free_exit.d \
			singleton.d \
			check_symbol.d \
			tokenization.d \
			signal_handling.d \
			expand.d \
			abstract_syntax_tree.d \
			remove_quote_before_exec.d \
			error.d \
			null.d)
NAME = minishell

all : $(NAME)

$(NAME): $(OBJ)
	make bonus -s -C $(LIB)
	$(CC) $(FLAG) $(OBJ) -lreadline -L $(LIB) -lft -o $(NAME)

obj/%.o: src/%.c | object
	$(CC) $(FLAG) -I $(LIB) -I $(HEAD) -c $< -o $@

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
