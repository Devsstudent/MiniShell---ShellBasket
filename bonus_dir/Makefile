FLAG = -Wall -g -Wextra -MMD -Werror
CC = gcc
LIB = ../libft
HEAD = ./includes
OBJ = $(addprefix obj/error/, error.o \
		free_exit.o \
		garbage_collector.o \
		signal_handling.o \
		singleton.o \
		error.o \
		garbage_collector_free.o) \
	$(addprefix obj/parsing, \
		abstract_syntax_tree_bonus.o \
		here_doc.o \
		here_doc_utils.o) \
	$(addprefix obj/utils/, \
		ast_utils.o \
		line_lst_utils.o \
		lst_utils.o) \
	$(addprefix obj/exec/,  \
		builtins.o \
		check_redir.o \
		cmd_not_found.o \
		exec.o \
		exec_subprocess.o \
		execve.o \
		forking.o \
 		handle_cmd_exec.o \
		handle_cmd_utils.o \
		handle_quote_before_exec.o \
		redit_ambiguous.o \
		redirection.o \
		remove_quote_before_exec.o \
		wait_process.o ) \
	$(addprefix obj/expand/, \
		expand.o \
		expand_check_key.o \
		expand_fill_word_val.o \
		expand_utils.o) \
	$(addprefix obj/lexing/, \
		check_symbol.o \
		copy_env.o \
		dict_to_double.o \
		lexing_bonus.o \
		lexing.o \
		lexing_fill_word.o \
		lexing_handle.o \
		lexing_parsing.o \
		tokenization_bonus.o) \
	$(addprefix obj/, \
		main.o \
		malloc_pid_func.o \
		wildcard_handler_new.o)
			
D_LST = $(OBJ:.o:=.d)

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
