/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/31 19:52:19 by odessein          #+#    #+#             */
/*   Updated: 2022/09/16 16:05:29 by mbelrhaz         ###   ########.fr       */
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
		write(2, "syntax error: unexpected token near field \n", 44);
}

void	print_error(char *ope, int type)
{
	(void) ope;
	if (type == 1)
	{
		write(2, "shellbasket: ambiguous redirect\n", 32);
		g_exit_status = 1;
	}
	if (type == 2)
	{
		ft_putstr_fd(ope, 2);
		write(2, ": command not found\n", 21);
		g_exit_status = 127;
	}
}

t_bool	perror_false(char *str)
{
	perror(str);
	g_exit_status = 1;
	return (FALSE);
}
