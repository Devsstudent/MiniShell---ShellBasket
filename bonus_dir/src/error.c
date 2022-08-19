#include "minishell.h"

char	*get_exit_status(void)
{
	return (ft_itoa(g_exit_status));
}

void	print_syntax_error(char *ope, t_bool quote)
{
	//Tokenization check pas les parenthese empty it should
	g_exit_status = 2;
	if (!quote)
		printf_error("syntax error: unclosed quote");
	else
		printf_error("syntax error: unexpected token nead field `%s`\n", ope);
}
