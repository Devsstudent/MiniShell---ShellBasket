/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_ambiguous.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: odessein <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/01 18:10:06 by odessein          #+#    #+#             */
/*   Updated: 2022/09/16 13:33:49 by odessein         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h"

void	ambiguous_case(t_bool type, t_info *exec_in)
{
	char	*msg;

	msg = "shellbasket: ambiguous redirect\n";
	if (type)
	{
		ft_putstr_fd("out", 2);
		exec_in->out_fd = -2;
	}
	if (!type)
	{
		ft_putstr_fd("open", 2);
		exec_in->open_fd = -2;
	}
	write(2, msg, ft_strlen(msg) + 1);
}

t_bool	check_ambiguous(char *word, t_info *exec_in, t_bool type)
{
	int		i;
	t_bool	quote;
	t_bool	d_quote;

	quote = FALSE;
	d_quote = FALSE;
	i = -1;
	while (word[++i])
	{
		check_quote_redir(&d_quote, &quote, word[i]);
		if (i > 0 && word[i - 1] != ' ' && word[i] == ' ' && !quote && !d_quote)
		{
			ambiguous_case(type, exec_in);
			return (TRUE);
		}
	}
	return (FALSE);
}
