#include "minishell.h"

char	*get_exit_status(void)
{
	return (ft_itoa(g_exit_status));
	//return (g_exit_status);
}

void	print_syntax_error(char *ope, int type)
{
	//Tokenization check pas les parenthese empty it should
	(void) ope;
	g_exit_status = 2;
	if (type == 1)
		write(2, "syntax error: unclosed quote\n", 30);
	else
		write(2, "syntax error: unexpected token near field \n", 45);
	//free_exit();
}

void	print_error(char *ope, int type)
{
	(void) ope;
	if (type == 1)
	{ //key not found in redirection
		write(2, "shellbasket: ambiguous redirect\n", 31);
		g_exit_status = 1;
	}
	if (type == 2)
	{
		write(2, "shellbasket: command not found\n", 31);
		g_exit_status = 127;
		//ft_putnbr_fd(g_exit_status, 2);
	}
		//ft_printf(1, "shellbasket: %s: command not found\n", ope);
}
