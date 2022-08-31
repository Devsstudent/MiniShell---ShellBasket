/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:52:19 by odessein          #+#    #+#             */
/*   Updated: 2022/08/31 19:52:22 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

char	*get_exit_status(void)
{
	return (ft_itoa(g_exit_status));
}

//Tokenization check pas les parenthese empty it should
void	print_syntax_error(char *ope, int type)
{
	(void) ope;
	g_exit_status = 2;
	if (type == 1)
		write(2, "syntax error: unclosed quote\n", 30);
	else
		write(2, "syntax error: unexpected token near field \n", 45);
}

void	print_error(char *ope, int type)
{
	(void) ope;
	if (type == 1)
	{
		write(2, "shellbasket: ambiguous redirect\n", 31);
		g_exit_status = 1;
	}
	if (type == 2)
	{
		write(2, "shellbasket: command not found\n", 31);
		g_exit_status = 127;
	}
}

t_bool	perror_false(char *str)
{
	perror(str);
	g_exit_status = errno;
	return (FALSE);
}
