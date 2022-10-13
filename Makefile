#don't forget the definition of CC, because default is cc, not gcc

CC = gcc
FLAG = -Wall -g3 -Wextra -MMD -Werror
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
			cd.o \
			echo.o \
			env.o \
			unset.o \
			exit.o \
			export.o \
			ft_strdup.o \
			pwd.o \
			handle_quote_before_exec.o \
			here_doc.o \
			forking.o \
			expand_check_key.o \
			expand_utils.o \
			expand_fill_word_val.o \
			dict_utils.o \
			dict_to_double.o \
			here_doc_utils.o \
			garbage_collector_free.o \
			lexing_handle.o \
			lexing_fill_word.o \
			handle_cmd_utils.o \
			export_utils.o \
			check_redir.o \
			redir_ambiguous.o \
			builtins.o \
			cmd_not_found.o \
			lexing_parsing.o \
			wait_process.o \
			malloc_pid_func.o \
			expand_utils_bis.o \
			handle_cmd_utils_bis.o \
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
			cd.d \
			echo.d \
			env.d \
			unset.d \
			exit.d \
			export.d \
			ft_strdup.d \
			pwd.d \
			here_doc.d \
			forking.d \
			handle_quote_before_exec.d \
			expand_check_key.d \
			expand_utils.d \
			expand_fill_word_val.d \
			dict_utils.d \
			dict_to_double.d \
			here_doc_utils.d \
			garbage_collector_free.d \
			lexing_fill_word.d \
			lexing_handle.d \
			export_utils.d \
			check_redir.d \
			redir_ambiguous.d \
			builtins.d \
			handle_cmd_utils.d \
			lexing_parsing.d \
			wait_process.d \
			cmd_not_found.d \
			malloc_pid_func.d \
			expand_utils_bis.d \
			handle_cmd_utils_bis.d \
			null.d)
NAME = minishell

all : $(NAME)

test: 
	(cd shelltester && bash ./tester.sh $(shell pwd)/minishell)

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
