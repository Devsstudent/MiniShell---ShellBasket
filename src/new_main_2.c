#include "minishell.h"

int	main(void)
{
	t_line	block_lst;

	block_lst.head = NULL;
	char *line = readline("word :");
	if (!fill_line_lst(&block_lst, line))
		ft_printf("malloc error");
}
