FLAG = -Wall -g -Wextra -MMD
LIB = ./libft
HEAD = ./includes
OBJ = $(addprefix obj/, main.o gc_collector.o copy_env.o lst_utils.o)
D_LST = $(addprefix obj/, main.d gc_collector.d copy_env.d lst_utils.d null.d)
NAME = minishell

all : $(NAME)

$(NAME): $(OBJ)
	make bonus -s -C $(LIB)
	$(CC) $(FLAG) $(OBJ) -lreadline -L $(LIB) -lft -o $(NAME)

obj/%.o: src/%.c | object
	$(CC) $(FLAG) -I $(LIB) -I $(HEAD) -lreadline -c $< -o $@

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
