FLAG = -Wall -g -Wextra -Werror -MMD
CC= gcc
LIB= ../libft
HEAD= ./includes
SRC= $(shell find ./src -name "*.c")
YO= $(SRC:.c=.o)
OBJ= $(YO:./src%=./obj%)
D_LST= $(OBJ:.o=.d)
NAME= minishell
OBJ= $(addprefix obj/, $(addsuffix .o, main \
	$(addprefix exec/,
		) \
	$(addprefix lexing/,
		) \
	$(addprefix expand/,
		) \
	$(addprefix error/,
		) \
	$(addprefix builtins/,
		) \
	$(addprefix parsing/,
		) \
	$(addprefix utils/, 
		) \
	$(addprefix wildcard/,
		)))
	
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
