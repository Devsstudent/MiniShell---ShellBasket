#include "minishell.h"

char	*get_exit_status(void)
{
	return (ft_itoa(g_exit_status));
}

void	print_syntax_error(char *ope, int type)
{
	//Tokenization check pas les parenthese empty it should
	g_exit_status = 2;
	if (type == 1)
		ft_printf(1, "syntax error: unclosed quote");
	else
		ft_printf(1, "syntax error: unexpected token near field `%s`\n", ope);
	free_exit();
}

void	print_error(char *ope, int type)
{
	if (type == 1) //key not found in redirection
		ft_printf(1, "shellbasket: ambiguous redirect");
	if (type == 2)
		ft_printf(1, "shellbasket: %s: command not found", ope);
}
