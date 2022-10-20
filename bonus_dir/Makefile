FLAG = -Wall -g -Wextra -Werror -MMD
LIB= ../libft
HEAD= ./includes
NAME= ../minishell_bonus
OBJ= $(addprefix obj/, $(addsuffix .o, main \
	$(addprefix exec/, builtins \
		exec_bonus_parentheses_func \
		check_redir \
		cmd_not_found \
		exec_bonus \
		exec_bonus_leaf_type \
		exec \
		exec_subprocess \
		exec_subshell \
		execve \
		forking \
		handle_cmd_exec \
		handle_quote_before_exec \
		redir_ambiguous \
		redirection \
		remove_quote_before_exec \
		wait_process) \
	$(addprefix lexing/, check_symbol \
		copy_env \
		dict_to_double \
		lexing_bonus \
		lexing_bonus_handle \
		lexing_fill_word \
		lexing_parsing \
		tokenization_bonus) \
	$(addprefix expand/, expand \
		expand_check_key \
		expand_fill_word_val) \
	$(addprefix error/, error \
		free_exit \
		garbage_collector \
		garbage_collector_free \
		signal_handling \
		singleton) \
	$(addprefix builtins/, cd \
		echo \
		env \
		exit \
		export \
		pwd \
		unset) \
	$(addprefix parsing/, abstract_syntax_tree_bonus \
		ast_bonus_fill \
		here_doc) \
	$(addprefix utils/, abstract_syntax_tree_bonus_utils \
		ast_utils \
		dict_utils \
		exec_info_utils \
		expand_utils_bis \
		expand_utils \
		export_utils \
		ft_strdup \
		handle_cmd_utils_bis \
		handle_cmd_utils \
		here_doc_utils \
		lexing_utils \
		line_lst_utils \
		lst_utils \
		pid_utils \
		wildcard_utils) \
	$(addprefix wildcard/, wildcard_check \
		wildcard_fill \
		wildcard_handle_quotes_utils \
		wildcard_handle_quotes \
		wildcard_handler)))

D_LST= $(OBJ:.o=.d)
all: $(NAME)

$(NAME): $(OBJ)
	@make bonus -s -C $(LIB)
	$(CC) $(FLAG) $(OBJ) -L /usr/local/opt/readline/lib  -lreadline -L $(LIB) -lft -o $(NAME)

test :
	(cd ../shelltester && bash ./tester.sh $(shell pwd)/minishell $(test_dir))

obj/%.o: src/%.c | object
	$(CC) $(FLAG) -I $(LIB) -I $(HEAD) -I /usr/local/opt/readline/include -c $< -o $@

object:
	@mkdir -p obj
	@mkdir -p obj/lexing
	@mkdir -p obj/expand
	@mkdir -p obj/exec
	@mkdir -p obj/parsing
	@mkdir -p obj/builtins
	@mkdir -p obj/utils
	@mkdir -p obj/error
	@mkdir -p obj/wildcard

clean:
	make -s clean -C $(LIB)
	rm -f $(NAME)

fclean:
	make -s fclean -C $(LIB)
	rm -f $(NAME)
	rm -rf obj

re : fclean all

.PHONY: all re clean fclean
-include $(D_LST)
