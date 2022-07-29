all:
	make -C libft
	make bonus -C libft
	gcc -Wall -Werror -Wextra -g main.c gc_collector.c -lreadline -Llibft -lft
