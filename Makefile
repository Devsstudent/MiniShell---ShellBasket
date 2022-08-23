#don't forget the definition of CC, because default is cc, not gcc

CC = gcc
FLAG = -Wall -g -Wextra -MMD -Werror
LIB = ./libft
HEAD = ./includes
OBJ = $(addprefix obj/, main.o \
			ast_utils.o \
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
			exec.o \
			exec_subprocess.o \
			execve.o \
			handle_cmd_exec.o \
			redirection.o \
			remove_quote_before_exec.o)
D_LST = $(addprefix obj/, main.d \
			ast_utils.d \
			garbage_collector.d \
			copy_env.d \
			lst_utils.d \
			lexing.d \
			line_lst_utils.d \
			free_exit.d \ singleton.d \
			check_symbol.d \
			tokenization.d \
			signal_handling.d \
			expand.d \
			abstract_syntax_tree.d \
			remove_quote_before_exec.d \
			error.d \
			exec_subprocess.d \
			execve.d \
			handle_cmd_exec.d \
			redirection.d \
			exec.d \
			null.d)
NAME = minishell

all : $(NAME)

$(NAME): $(OBJ)
	make bonus -s -C $(LIB)
	$(CC) $(FLAG) $(OBJ) -L /usr/local/opt/readline/lib  -lreadline -L $(LIB) -lft -o $(NAME)

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
