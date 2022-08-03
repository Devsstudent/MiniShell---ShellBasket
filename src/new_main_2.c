#include "minishell.h"

int	main(void)
{
	while (1)
	{
		t_line	block_lst;
		
		block_lst.head = NULL;
		char *line = readline("word :");
		if (!line)
			return (1);
		if (!fill_line_lst(&block_lst, line))
			ft_printf("malloc error");
		if (!tokenisation(&block_lst))
			ft_printf("syntax error");
		
	}
}
